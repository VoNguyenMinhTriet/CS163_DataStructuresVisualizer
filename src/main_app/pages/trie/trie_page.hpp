#pragma once

#include "../page.hpp"
#include "main_app/pages/trie/trie_scene.hpp"
#include "main_app/pages/main_menu.hpp"
#include "raylib-cpp/Font.hpp"
#include "raylib-cpp/Functions.hpp"
#include "raylib-cpp/Rectangle.hpp"
#include "raylib-cpp/Texture.hpp"
#include "raylib-cpp/Vector2.hpp"
#include "widget_toolkit/controls/button.hpp"
#include "widget_toolkit/controls/text_box.hpp"
#include "widget_toolkit/controls/code_box.hpp"
#include <fstream>
#include <functional>
#include <future>
#include <string>
#include <vector>
#include <algorithm>

namespace ds_viz::pages
{
class TriePage : public Page
{
  private:
    std::vector<std::string> wordList { 
        // passwords for the lulz, some are explicit and some are for dum-dums: 
        // https://github.com/kkrypt0nn/wordlists/blob/main/wordlists/passwords/common_passwords_win.txt
        "aaa",           "abc",         "academia",   "academic",
        "access",        "ada",         "admin",      "adrian",
        "adrianna",      "aerobics",    "airplane",   "albany",
        "albatross",     "albert",      "alex",       "alexander",
        "alf",           "algebra",     "alias",      "aliases",
        "alice",         "alicia",      "alisa",      "alison",
        "allison",       "alpha",       "alphabet",   "ama",
        "amadeus",       "amanda",      "amber",      "amorphous",
        "amy",           "analog",      "anchor",     "andrea",
        "andromache",    "andy",        "angela",     "angerine",
        "angie",         "animals",     "anita",      "ann",
        "anna",          "anne",        "annette",    "answer",
        "anthropogenic", "anvils",      "anything",   "april",
        "aria",          "ariadne",     "arlene",     "arrow",
        "arthur",        "asd",         "asm",        "asshole",
        "athena",        "atmosphere",  "aztecs",     "azure",
        "bacchus",       "badass",      "bailey",     "banana",
        "bananas",       "bandit",      "banks",      "barbara",
        "barber",        "baritone",    "bart",       "bartman",
        "basic",         "bass",        "bassoon",    "batch",
        "batman",        "beach",       "beater",     "beauty",
        "beaver",        "becky",       "beethoven",  "beloved",
        "benz",          "beowulf",     "berkeley",   "berlin",
        "berliner",      "beryl",       "beta",       "beth",
        "betsie",        "betty",       "beverly",    "bicameral",
        "bishop",        "bitch",       "bob",        "bradley",
        "brandi",        "brandy",      "brenda",     "brian",
        "bridget",       "broadway",    "bsd",        "bumbling",
        "burgess",       "cad",         "camille",    "campanile",
        "candi",         "candy",       "cantor",     "cardinal",
        "caren",         "carla",       "carmen",     "carol",
        "carole",        "carolina",    "caroline",   "carrie",
        "carson",        "cascades",    "castle",     "cat",
        "catherine",     "cathy",       "cayuga",     "cecily",
        "celtics",       "cerulean",    "change",     "charity",
        "charles",       "charming",    "charon",     "chat",
        "chem",          "chemistry",   "chess",      "chester",
        "christina",     "christine",   "christy",    "cigar",
        "cindy",         "class",       "classic",    "claudia",
        "cluster",       "clusters",    "code",       "coffee",
        "coke",          "collins",     "commrades",  "computer",
        "comrade",       "comrades",    "condo",      "condom",
        "connect",       "connie",      "console",    "cookie",
        "cooper",        "cornelius",   "couscous",   "create",
        "creation",      "creosote",    "cretin",     "criminal",
        "cristina",      "crystal",     "cshrc",      "cynthia",
        "daemon",        "daisy",       "dana",       "dancer",
        "daniel",        "danielle",    "danny",      "dapper",
        "data",          "dave",        "dawn",       "deb",
        "debbie",        "deborah",     "december",   "default",
        "defoe",         "deluge",      "denise",     "desiree",
        "desperate",     "develop",     "device",     "dial",
        "diana",         "diane",       "diet",       "dieter",
        "digital",       "disc",        "discovery",  "disk",
        "disney",        "dog",         "dos",        "drought",
        "dulce",         "duncan",      "eager",      "earth",
        "easier",        "easy",        "eatme",      "edges",
        "edinburgh",     "edwin",       "edwina",     "egghead",
        "eiderdown",     "eileen",      "einstein",   "elaine",
        "elanor",        "elephant",    "elizabeth",  "ellen",
        "email",         "emerald",     "emily",      "emmanuel",
        "enemy",         "engine",      "engineer",   "enterprise",
        "enzyme",        "erenity",     "erica",      "erika",
        "erin",          "ersatz",      "establish",  "estate",
        "eternity",      "euclid",      "evelyn",     "extension",
        "fairway",       "felicia",     "fender",     "fermat",
        "ferrari",       "fidelity",    "field",      "file",
        "finite",        "fishers",     "flakes",     "float",
        "flower",        "flowers",     "foolproof",  "football",
        "foresight",     "format",      "forsythe",   "fourier",
        "fred",          "friend",      "frighten",   "fun",
        "function",      "fungible",    "gabriel",    "games",
        "gardner",       "garfield",    "gatt",       "gauss",
        "george",        "gertrude",    "gibson",     "gina",
        "ginger",        "glacier",     "gnu",        "golf",
        "golfer",        "gorgeous",    "gorges",     "gosling",
        "gouge",         "graham",      "grahm",      "group",
        "gryphon",       "gucci",       "guess",      "guest",
        "guitar",        "gumption",    "guntis",     "hack",
        "hacker",        "hal",         "hamlet",     "handily",
        "happening",     "harmony",     "harold",     "harvey",
        "hawaii",        "heather",     "hebrides",   "heidi",
        "heinlein",      "hello",       "help",       "herbert",
        "hiawatha",      "hibernia",    "hidden",     "holly",
        "homework",      "honey",       "horse",      "horus",
        "hutchins",      "hydrogen",    "ibm",        "imbroglio",
        "imperial",      "include",     "ingres",     "ingress",
        "ingrid",        "inna",        "innocuous",  "internet",
        "irene",         "irishman",    "isis",       "jackie",
        "jane",          "janet",       "janice",     "janie",
        "japan",         "jasmin",      "jean",       "jeanne",
        "jen",           "jenni",       "jennifer",   "jenny",
        "jessica",       "jester",      "jill",       "jixian",
        "joanne",        "jody",        "johnny",     "joseph",
        "joshua",        "joy",         "joyce",      "judith",
        "judy",          "juggle",      "julia",      "julie",
        "june",          "jupiter",     "karen",      "karie",
        "karina",        "kate",        "kathleen",   "kathrine",
        "kathy",         "katina",      "katrina",    "kelly",
        "keri",          "kermit",      "kernel",     "kerri",
        "kerrie",        "kerry",       "key",        "kim",
        "kimberly",      "kirkland",    "kitten",     "knight",
        "krista",        "kristen",     "kristi",     "kristie",
        "kristin",       "kristine",    "kristy",     "ladle",
        "lambda",        "lamination",  "lana",       "lara",
        "larkin",        "larry",       "laura",      "lazarus",
        "leah",          "lebesgue",    "lee",        "leland",
        "leroy",         "leslie",      "lewis",      "library",
        "light",         "linda",       "lisa",       "lisp",
        "liz",           "lock",        "lockout",    "lois",
        "lori",          "lorin",       "lorraine",   "louis",
        "love",          "lucy",        "lynn",       "lynne",
        "macintosh",     "mack",        "maggot",     "magic",
        "mail",          "maint",       "malcolm",    "malcom",
        "manager",       "mara",        "marci",      "marcy",
        "maria",         "marietta",    "mark",       "markus",
        "marni",         "mars",        "marty",      "marvin",
        "mary",          "master",      "math",       "maurice",
        "meagan",        "megan",       "melissa",    "mellon",
        "memory",        "mercury",     "merlin",     "mets",
        "mgr",           "michael",     "michele",    "michelle",
        "mickey",        "mike",        "minimum",    "minsky",
        "mit",           "modem",       "mogul",      "moguls",
        "monica",        "moose",       "morley",     "mouse",
        "mozart",        "mutant",      "nagel",      "nancy",
        "napoleon",      "nasa",        "nepenthe",   "neptune",
        "ness",          "net",         "network",    "new",
        "news",          "newton",      "next",       "nicole",
        "nita",          "nobody",      "noreen",     "noxious",
        "nuclear",       "nutrition",   "nyquist",    "oceanography",
        "ocelot",        "office",      "olivetti",   "olivia",
        "open",          "operator",    "oracle",     "orca",
        "orwell",        "osiris",      "outlaw",     "oxford",
        "pacific",       "pad",         "painless",   "pakistan",
        "pam",           "pamela",      "paper",      "papers",
        "pass",          "password",    "pat",        "patricia",
        "patty",         "paula",       "pencil",     "penelope",
        "penguin",       "penis",       "peoria",     "percolate",
        "persimmon",     "persona",     "pete",       "peter",
        "philip",        "phoenix",     "phone",      "pierre",
        "pizza",         "plane",       "playboy",    "plover",
        "pluto",         "plymouth",    "polly",      "polynomial",
        "pondering",     "pork",        "porsche",    "poster",
        "power",         "praise",      "precious",   "prelude",
        "presto",        "prince",      "princeton",  "priv",
        "private",       "privs",       "professor",  "profile",
        "program",       "protect",     "protozoa",   "pub",
        "public",        "pumpkin",     "puneet",     "puppet",
        "qwerty",        "rabbit",      "rachel",     "rachelle",
        "rachmaninoff",  "rainbow",     "raindrop",   "raleigh",
        "random",        "rascal",      "reagan",     "really",
        "rebecca",       "regional",    "remote",     "renee",
        "rick",          "ripple",      "risc",       "rje",
        "robin",         "robot",       "robotics",   "robyn",
        "rochelle",      "rochester",   "rodent",     "rolex",
        "romano",        "ronald",      "root",       "rose",
        "rosebud",       "rosemary",    "roses",      "ruben",
        "rules",         "ruth",        "sal",        "samantha",
        "sandra",        "sandy",       "sara",       "sarah",
        "saturn",        "saxon",       "scamper",    "scheme",
        "school",        "scott",       "scotty",     "secret",
        "security",      "sensor",      "serenity",   "service",
        "sesame",        "sex",         "shannon",    "sharc",
        "shark",         "sharks",      "sharon",     "sheffield",
        "sheldon",       "shell",       "sherri",     "shirley",
        "shit",          "shiva",       "shivers",    "shuttle",
        "signature",     "simon",       "simple",     "simpsons",
        "singer",        "single",      "smile",      "smiles",
        "smooch",        "smother",     "snatch",     "snoopy",
        "soap",          "socrates",    "somebody",   "sondra",
        "sonia",         "sonya",       "sossina",    "sparrows",
        "spit",          "spring",      "springer",   "squires",
        "stacey",        "staci",       "stacie",     "stacy",
        "steph",         "stephanie",   "strangle",   "stratford",
        "student",       "stuttgart",   "subway",     "success",
        "summer",        "sun",         "super",      "superstage",
        "superuser",     "support",     "supported",  "surfer",
        "susan",         "susanne",     "susie",      "suzanne",
        "suzie",         "swearer",     "sybil",      "symmetry",
        "sys",           "sysadmin",    "system",     "tamara",
        "tami",          "tamie",       "tammy",      "tangerine",
        "tape",          "tara",        "target",     "tarragon",
        "taylor",        "tech",        "telephone",  "temptation",
        "tennis",        "terminal",    "test",       "thailand",
        "theresa",       "tiffany",     "tiger",      "tina",
        "toggle",        "tomato",      "topography", "tortoise",
        "toxic",         "toyota",      "traci",      "tracie",
        "tracy",         "trails",      "transfer",   "trisha",
        "trivial",       "trombone",    "tty",        "tubas",
        "tuttle",        "umesh",       "unhappy",    "unicorn",
        "unix",          "unknown",     "uranus",     "urchin",
        "ursula",        "util",        "utility",    "uucp",
        "valerie",       "vasant",      "venus",      "veronica",
        "vertigo",       "vicky",       "village",    "virgin",
        "virginia",      "visitor",     "wargames",   "warren",
        "water",         "weenie",      "wendi",      "wendy",
        "whatever",      "whatnot",     "whiting",    "whitney",
        "wholesale",     "will",        "william",    "williamsburg",
        "willie",        "wilma",       "winston",    "wisconsin",
        "wizard",        "wombat",      "woodwind",   "word",
        "work",          "wormwood",    "wyoming",    "xfer",
        "xmodem",        "xyz",         "xyzzy",      "yaco",
        "yang",          "yellowstone", "yolanda",    "yosemite",
        "zap",           "zimmerman",   "zmodem",
    };

    float stepsPerSecond = 4;
    bool actionsShown = false;

    bool _loadingFile = false;
    std::future<void> _loadTask;

    bool _isPlaying = false;
    float _currentTime = 0;

    std::unique_ptr<raylib::Font> titleFont;
    raylib::Text title;

    // Controls

    raywtk::Button _homeButton;
    raylib::Texture homeIcon = raylib::Texture("images/return_button.png");

    raywtk::Button _toggleShowActionsButton;
    raywtk::Button _initializeButton;
    raywtk::Button _searchButton;
    raywtk::Button _addButton;
    raywtk::Button _removeButton;
    raywtk::Button _clearButton;

    raywtk::TextBox _searchTextBox; bool _searchBoxVisible = false;
    raywtk::TextBox _addTextBox; bool _addBoxVisible = false;
    raywtk::TextBox _removeTextBox; bool _removeBoxVisible = false;
    
    raywtk::Button _showCodeBoxButton;
    raywtk::CodeBox _codeBox; bool _codeBoxVisible = false;
    
    raywtk::TextBox _stepsPerSecTextBox;
    raywtk::Button _prevStepButton;
    raywtk::Button _nextStepButton;
    raywtk::Button _playPauseButton;

    trie::TrieScene _scene;

    std::unique_ptr<raylib::Font> buttonFont = std::make_unique<raylib::Font>("./ttf/Inter-Regular.ttf", 16);

    void RenderTextInCenter (const raylib::Rectangle& rect, const raylib::Font& font,
        const std::string& text, raylib::Color color)
    {
        auto txt = raylib::Text(text, font.GetBaseSize(), color, font, 0);
        txt.Draw(rect.x + rect.width / 2 - txt.MeasureEx().x / 2,
            rect.y + rect.height / 2 - txt.MeasureEx().y / 2);
    }

    void RenderTextOnRight (const raylib::Vector2& topRightAnchor, const raylib::Font& font,
        const std::string& text, raylib::Color color)
    {
        auto txt = raylib::Text(text, font.GetBaseSize(), color, font, 0);
        txt.Draw(topRightAnchor.x - txt.MeasureEx().x, topRightAnchor.y);
    }

    void ToggleShowActions()
    {
        actionsShown = !actionsShown;
    }

    void InitializeRandomly() 
    {
        std::vector<int> chosenWords;
        _scene.root = std::make_unique<trie::TrieNode>();
        for (int i = 0; i < 10; ++i) 
        {    
            int randomIndex;
            do randomIndex = GetRandomValue(0, wordList.size() - 1);
                while (std::find(chosenWords.begin(), chosenWords.end(), randomIndex) != chosenWords.end());
            chosenWords.push_back(randomIndex);
            _scene.root->Add(wordList[randomIndex]);
        }
    }

    void LoadFileIfDropped()
    {
        auto dropped = raylib::LoadDroppedFiles();
        if (dropped.size() != 1) return;
        
        _loadingFile = true;
        _loadTask = std::async(std::launch::async, [this, dropped]() mutable {
            std::ifstream file(dropped[0]);
            if (!file.is_open()) return;

            std::string line;
            while (std::getline(file, line))
            {
                if (line.empty()) continue;
                _scene.root->Add(line);
            }
            _loadingFile = false;
        });
    }

    void GoBackHome();

  public:
    TriePage(MainWindow& context);

    void Update(float deltaTime) override;
    void Render() override;
};
} // namespace ds_viz::pages
