#include "Level.h"

using namespace std::string_view_literals;

constexpr inline const std::array<StringView, 3> LEVEL_CONFIG_PATHS = 
{ 
    "resources\\levels\\level_01.xml"sv,
    "resources\\levels\\level_02.xml"sv,
    "resources\\levels\\level_03.xml"sv
};
constexpr inline const StringView PADDLE_TEXTURE_PATH = "resources\\textures\\paddle.png"sv;
constexpr inline const StringView BALL_TEXTURE_PATH = "resources\\textures\\ball2.png"sv;
constexpr inline const StringView BRICK_TEXTURES_PATH_FORMAT = "resources\\textures\\bricks\\{}.png"sv;
constexpr inline const StringView BRICK_OUT_OF_RANGE_MSG_FORMAT = "Trying to fetch brick with out of range indexes! Row(i): {}, Column(j): {}\n"sv;
constexpr inline const StringView CONFIG_PARSING_ERROR_FORMAT = "Error while reading xml level config! \nCause: {}"sv;
constexpr inline const StringView INVALID_CONFIGURATION_MSG = "Brick configuration does not match row and column count! Check level config xml..."sv;
constexpr inline const Vector4<int> INITIAL_PLAYER_TRANSFORM_VALUES = { 400, 550, 50, 10 };
constexpr inline const Vector4<int> INITIAL_BALL_TRANSFORM_VALUES = { 400, 530, 20, 20 };

int Level::getRowCount() const { return _rowCount; }
int Level::getColumnCount() const { return _columnCount; }
int Level::getRowSpacing() const { return _rowSpacing; }
int Level::getColumnSpacing() const { return _columnSpacing; }
int Level::getBrickWidth() const { return _brickWidth; }
int Level::gteBrickHeight() const { return _brickHeight; }
CharMatrix Level::getBricks() const { return _bricks; }

Level::Level(int level) 
{ 
    if (level < 1 || level > 3)
        throw std::invalid_argument("Invalid level number selected. Levels 1, 2, and 3 are available!");
    
    loadConfig(LEVEL_CONFIG_PATHS[level-1]);
}

void Level::loadConfig(const Path& path)
{
    TiXmlDocument file(path.string().data());
    file.LoadFile();

    TiXmlHandle levelConfig(&file);

    TiXmlElement* level = levelConfig.FirstChild("level").ToElement();

    TiXmlElement const* rowCount = level->FirstChildElement("rowCount");
    TiXmlElement const* columnCount = level->FirstChildElement("columnCount");
    TiXmlElement const* rowSpacing = level->FirstChildElement("rowSpacing");
    TiXmlElement const* columnSpacing = level->FirstChildElement("columnSpacing");
    TiXmlElement const* backgroundTexture = level->FirstChildElement("backgroundTexture");
    TiXmlElement const* bricks = level->FirstChildElement("bricks");
    TiXmlElement const* brickTypes = level->FirstChildElement("brickTypes");

    try
    {
        _rowCount = std::stoi(rowCount->GetText());
        _columnCount = std::stoi(columnCount->GetText());
        _rowSpacing = std::stoi(rowSpacing->GetText());
        _columnSpacing = std::stoi(columnSpacing->GetText());
        _backgroundTexture = backgroundTexture->GetText();
        _bricks = CreateCharMatrix(bricks->GetText());
        auto brickType = brickTypes->FirstChildElement("brickType");

        while (brickType)
        {
            
            auto id = brickType->FirstChildElement("id")->GetText();
            auto texture =  brickType->FirstChildElement("texture")->GetText();
            auto hitPoints = brickType->FirstChildElement("hitPoints")->GetText();
            auto hitSound = brickType->FirstChildElement("hitSound")->GetText();
            auto breakSound = brickType->FirstChildElement("breakSound")->GetText();
            auto breakScore = brickType->FirstChildElement("breakScore")->GetText();

            if (id == nullptr || texture == nullptr || hitPoints == nullptr || hitSound == nullptr)
                throw std::invalid_argument("Invalid level config! Following properties can't be null: id, texture, hitPoints, hitSound");

            if (breakSound == nullptr) breakSound = "";
            if (breakScore == nullptr) breakScore = "0";

            auto type = BrickType(*id, texture, hitPoints, hitSound, breakSound, breakScore);
            _brickTypes.push_back(type);
            brickType = brickType->NextSiblingElement();

            _brickTypes.push_back(type);
        }
    }
    catch (const std::invalid_argument& e) { std::cout << std::format(CONFIG_PARSING_ERROR_FORMAT, e.what()); }
    catch (const std::out_of_range& e){ std::cout << std::format(CONFIG_PARSING_ERROR_FORMAT, e.what());}
}

std::optional<BrickType> Level::getBrickType(char t) const
{
    std::optional<BrickType> result = {};
    for (auto& type : _brickTypes)
        if (type.getId() == t)
            return result.emplace(type);

    return result;
}

std::optional<char> Level::getBrickType(int i, int j) const
{
    try
    {
        return { getBricks().at(i).at(j) };
    }
    catch (const std::out_of_range&)
    {
        std::cout << std::format(BRICK_OUT_OF_RANGE_MSG_FORMAT, i, j);
    }
    return {};
}

std::pair<int, int> Level::getBrickScreenPosition(int i, int j, int brickWidth, int brickHeight, int columnSpacing, int rowSpacing) const
{
    if (bool areInvalidIndexes = !(i < _rowCount && j < _columnCount))
    {

        std::cout << std::format(BRICK_OUT_OF_RANGE_MSG_FORMAT, i, j, _rowCount, _columnCount);
    }

    int x = columnSpacing + j * (brickWidth + columnSpacing);
    int y = rowSpacing + i * (brickHeight + rowSpacing);

    return { x, y };
}

bool Level::isEmptySpace(int i, int j) const { return getBrickType(i, j).value_or('\0') == '_'; }

std::optional<String> Level::getBrickTexturePath(int i, int j) const
{
    auto brickTypeOpt = getBrickType(i, j);

    if (!brickTypeOpt.has_value()) 
        return {};

    std::optional<String> result {};

    switch (brickTypeOpt.value()) {
    case 'H':
        result.emplace(std::format(BRICK_TEXTURES_PATH_FORMAT, "hard"));
        break;
    case'M':
        result.emplace(std::format(BRICK_TEXTURES_PATH_FORMAT, "medium"));
        break;
    case'S':
        result.emplace(std::format(BRICK_TEXTURES_PATH_FORMAT, "soft"));
        break;
    case'I':
        result.emplace(std::format(BRICK_TEXTURES_PATH_FORMAT, "infinite"));
        break;
    default:
        break;
    }

    return result;
}

void Level::initializeBricks(GameManager& manager, const Window& window) const
{
    using enum ComponentType;

    const int windowWidth = window.getWidth();
    const int windowHeight = window.getHeight();
    const int cameraSpaceSize = 100;
    const int brickHeight = 10;

    for (int i = 0; i < getRowCount(); ++i)
        for (int j = 0; j < getColumnCount(); ++j)
        {
            if (isEmptySpace(i, j)) continue;

            int columnSpacing = getColumnSpacing() * windowWidth / cameraSpaceSize;
            int rowSpacing = getRowSpacing() * windowHeight / cameraSpaceSize;

            int w = (windowWidth - (columnSpacing * (getColumnCount() + 1))) / getColumnCount();
            int h = brickHeight;
            auto [x, y] = getBrickScreenPosition(i, j, w, h, columnSpacing, rowSpacing);

            auto brickObject = manager.createGameObject();
            brickObject.setTag("brick");

            if (auto brickOpt = getBrickType(i, j); brickOpt.has_value())
            {
                int health = 0; 
                int points = 0;
                for (auto& type : _brickTypes)
                    if (type.getId() == brickOpt.value())
                    {
                        health = type.getHitPoints();
                        points = type.getBreakScore();
                    }

                brickObject.setProperty("health", health);
                brickObject.setProperty("points", points);
                brickObject.setProperty("brickType", brickOpt.value());

                auto texturePathOpt = getBrickTexturePath(i, j);
                auto* brickTexture = new Texture(window.getRenderer(), texturePathOpt.value());
                manager.addComponentToObject(brickObject, brickTexture);

                Transform* transform = nullptr;
                if (auto transformOpt = brickObject.getComponentPtr<Transform>(ComponentType::TRANSFORM); transformOpt.has_value()) {
                    transform = transformOpt.value();
                    transform->set(x, y, w, h);

                    auto* brickCollider = new BoxCollider(transform, w, h);
                    manager.addComponentToObject(brickObject, brickCollider);
                }
            }
            manager.getGameObjects().push_back(brickObject);
        }
}

void Level::initializePlayer(GameManager& manager, const Window& window) const
{
    using enum ComponentType;

    auto paddle = manager.createGameObject();
    paddle.setTag("player");

    auto* texture = new Texture(window.getRenderer(), PADDLE_TEXTURE_PATH);
    manager.addComponentToObject(paddle, texture);

    if (auto transformOpt = paddle.getComponentPtr<Transform>(ComponentType::TRANSFORM); transformOpt.has_value())
    {
        auto transform = transformOpt.value();
        transform->set(INITIAL_PLAYER_TRANSFORM_VALUES);

        auto [x, y, w, h] = INITIAL_PLAYER_TRANSFORM_VALUES;
        auto* paddleCollider = new BoxCollider(transform, w, h);
        manager.addComponentToObject(paddle, paddleCollider);
    }
    manager.getGameObjects().push_back(paddle);
}

void Level::initializeBall(GameManager& manager, const Window& window) const
{
    using enum ComponentType;

    auto ball = manager.createGameObject();
    ball.setTag("ball");

    auto* texture = new Texture(window.getRenderer(), BALL_TEXTURE_PATH);
    manager.addComponentToObject(ball, texture);

    Transform* transform = nullptr;
    if (auto transformOpt = ball.getComponentPtr<Transform>(ComponentType::TRANSFORM); transformOpt.has_value())
    {
        transform = transformOpt.value();
        transform->set(INITIAL_BALL_TRANSFORM_VALUES);
        manager.addComponentToObject(ball, transform);

        auto* force = new Force(1, -2);
        manager.addComponentToObject(ball, force);
    }

    auto [x, y, w, h] = INITIAL_BALL_TRANSFORM_VALUES;
    auto* ballCollider = new CircleCollider(transform, w/2);
    manager.addComponentToObject(ball, ballCollider);

    manager.getGameObjects().push_back(ball);

}

CharMatrix Level::CreateCharMatrix(String config) const
{
    auto brickMatrix = Vector<Vector<char>>(_rowCount);

    std::erase_if(config, ::isspace);

    if (config.length() != _rowCount * _columnCount)
    {
        std::cout << INVALID_CONFIGURATION_MSG;
    }

    for (int i = 0; auto& matrixRow : brickMatrix)
    {
        auto row = Vector<char>(_columnCount);
        for (int j = 0; auto& element : row)
        {
            element = config.at((_columnCount * i) + j);
            ++j;
        }
        matrixRow = row;
        ++i;
    }

    return brickMatrix;
}

void Level::resetBall(const GameObject& ball) const
{
    ball.getComponentPtr<Transform>(ComponentType::TRANSFORM).value()->set(INITIAL_BALL_TRANSFORM_VALUES);
}

void Level::resetPlayer(const GameObject& player) const
{
    player.getComponentPtr<Transform>(ComponentType::TRANSFORM).value()->set(INITIAL_PLAYER_TRANSFORM_VALUES);
}

template <typename T>
std::optional<T*> GameObject::getComponentPtr(ComponentType type) const
{
    if (std::optional<Component*> componentOpt = getComponentPtr(type); componentOpt.has_value())
        try
    {
        return std::optional<T*>(dynamic_cast<T*>(componentOpt.value()));
    }
    catch (const std::bad_cast& e)
    {
        std::cout << "Bad component cast! Casting from Component* to specific type\n" << e.what();
    }
    return {};
}
