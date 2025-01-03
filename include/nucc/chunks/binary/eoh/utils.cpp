#include "utils.hpp"

namespace nucc {
    namespace EOH {

std::unordered_map<std::string, std::string> character_id_reflist = {
    {"Baoh",                            "0bao01"},
    {"Ikuro Hashizawa",                 "0bao01"},
    {"Ikuro",                           "0bao01"},
    {"Hashizawa",                       "0bao01"},
    // Part 1: Phantom Blood
    {"Dio Brando",                      "1dio01"},
    {"Dio",                             "1dio01"},
    {"Dio Brando (Part 1)",             "1dio01"},
    {"Dio (Part 1)",                    "1dio01"},
    {"Dio Brando 1",                    "1dio01"},
    {"Dio 1",                           "1dio01"},
    {"Jonathan Joestar",                "1jnt01"},
    {"Jonathan",                        "1jnt01"},
    {"Robert E. O. Speedwagon",         "1sdw01"},
    {"Speedwagon",                      "1sdw01"},
    {"REO Speedwagon",                  "1sdw01"},
    {"R.E.O. Speedwagon",               "1sdw01"},
    {"Robert Speedwagon",               "1sdw01"},
    {"Will A. Zeppeli",                 "1zpl01"},
    {"Will A Zeppeli",                  "1zpl01"},
    {"Will Anthonio Zeppeli",           "1zpl01"},
    {"William Anthonio Zeppeli",        "1zpl01"},
    {"Will Antonio Zeppeli",            "1zpl01"},
    {"William Antonio Zeppeli",         "1zpl01"},
    {"William A. Zeppeli",              "1zpl01"},
    {"William A Zeppeli",               "1zpl01"},
    {"Baron Zeppeli",                   "1zpl01"},
    {"Zeppeli",                         "1zpl01"},
    // Part 2: Battle Tendency
    {"Caesar Anthonio Zeppeli",         "2csr01"},
    {"Caesar Antonio Zeppeli",          "2csr01"},
    {"Caesar Zeppeli",                  "2csr01"},
    {"Caesar",                          "2csr01"},
    {"Caesar A Zeppeli",                "2csr01"},
    {"Caesar A. Zeppeli",               "2csr01"},
    {"Esidisi",                         "2esd01"},
    {"Joseph Joestar",                  "2jsp01"},
    {"Joseph",                          "2jsp01"},
    {"Joseph Joestar (Part 2)",         "2jsp01"},
    {"Joseph (Part 2)",                 "2jsp01"},
    {"Joseph Joestar 2",                "2jsp01"},
    {"Joseph 2",                        "2jsp01"},
    {"2seph",                           "2jsp01"},
    {"Lisa Lisa (Elizabeth)",           "2lsa01"},
    {"Lisa Lisa",                       "2lsa01"},
    {"Elizabeth Joestar",               "2lsa01"},
    {"Elizabeth",                       "2lsa01"},
    {"Kars",                            "2krs01"},
    {"Rudol von Stroheim",              "2shm01"},
    {"Stroheim",                        "2shm01"},
    {"Von Stroheim",                    "2shm01"},
    {"Wamuu",                           "2wmu01"},
    // Part 3: Stardust Crusaders
    {"Muhammad Avdol",                  "3abd01"},
    {"Mohammed Avdol",                  "3abd01"},
    {"Muhammad Abdul",                  "3abd01"},
    {"Mohammed Abdul",                  "3abd01"},
    {"Avdol",                           "3abd01"},
    {"Abdul",                           "3abd01"},
    {"DIO",                             "3dio01"},
    {"Dio Brando (Part 3)",             "3dio01"},
    {"Dio (Part 3)",                    "3dio01"},
    {"Dio Brando 3",                    "3dio01"},
    {"Dio 3",                           "3dio01"},
    {"Hol Horse",                       "3hhs01"},
    {"Iggy",                            "3igy01"},
    {"Old Joseph Joestar",              "3jsp01"},
    {"Old Joseph",                      "3jsp01"},
    {"Oldseph",                         "3jsp01"},
    {"Joseph Joestar (Part 3)",         "3jtr01"},
    {"Joseph (Part 3)",                 "3jtr01"},
    {"Joseph Joestar 3",                "3jtr01"},
    {"Joseph 3",                        "3jtr01"},
    {"3seph",                           "3jtr01"},
    {"Jotaro Kujo",                     "3jtr01"},
    {"Jotaro",                          "3jtr01"},
    {"Jotaro Kujo (Part 3)",            "3jtr01"},
    {"Jotaro (Part 3)",                 "3jtr01"},
    {"Jotaro Kujo 3",                   "3jtr01"},
    {"Jotaro 3",                        "3jtr01"},
    {"3taro",                           "3jtr01"},
    {"Noriaki Kakyoin",                 "3kki01"},
    {"Kakyoin",                         "3kki01"},
    {"Noriaki",                         "3kki01"},
    {"Mariah",                          "3mra01"},
    {"Jean Pierre Polnareff",           "3pln01"},
    {"Jean-Pierre Polnareff",           "3pln01"},
    {"Polnareff",                       "3pln01"},
    {"Jean Pierre",                     "3pln01"},
    {"Jean-Pierre",                     "3pln01"},
    {"Pet Shop",                        "3psp01"},
    {"Vanilla Ice",                     "3vni01"},
    // Part 4: Diamond is Unbreakable
    {"Yuya Fungami",                    "4fgm01"},
    {"Yuya",                            "4fgm01"},
    {"Fungami",                         "4fgm01"},
    {"Josuke Higashikata",              "4jsk01"},
    {"Josuke",                          "4jsk01"},
    {"Josuke Higashikata 4",            "4jsk01"},
    {"Josuke 4",                        "4jsk01"},
    {"4suke",                           "4jsk01"},
    {"Jotaro Kujo (Part 4)",            "4jtr01"},
    {"Jotaro Kujo 4",                   "4jtr01"},
    {"Jotaro (Part 4)",                 "4jtr01"},
    {"Jotaro 4",                        "4jtr01"},
    {"4taro",                           "4jtr01"},
    {"Keicho Nijimura",                 "4kch01"},
    {"Keicho",                          "4kch01"},
    {"Yoshikage Kira",                  "4kir01"},
    {"Kira",                            "4kir01"},
    {"Yoshikage",                       "4kir01"},
    {"Koichi Hirose",                   "4koi01"},
    {"Koichi",                          "4koi01"},
    {"Kosaku Kawajiri",                 "4kwk01"},
    {"Kosaku",                          "4kwk01"},
    {"Okuyasu Nijimura",                "4oky01"},
    {"Okuyasu",                         "4oky01"},
    {"Akira Otoishi",                   "4oti01"},
    {"Akira",                           "4oti01"},
    {"Otoishi",                         "4oti01"},
    {"Rohan Kishibe",                   "4rhn01"},
    {"Rohan",                           "4rhn01"},
    {"Kishibe",                         "4rhn01"},
    {"Shigekiyo Yangu",                 "4sgc01"},
    {"Shigekiyo",                       "4sgc01"},
    {"Shigechi",                        "4sgc01"},
    {"Yukako Yamagishi",                "4ykk01"},
    {"Yukako",                          "4ykk01"},
    {"Yamagishi",                       "4ykk01"},
    // Parte 5: Vento Aureo
    {"Leone Abbacchio",                 "5abc01"},
    {"Abbacchio",                       "5abc01"},
    {"Leone",                           "5abc01"},
    {"Bruno Bucciarati",                "5bct01"},
    {"Bucciarati",                      "5bct01"},
    {"Bruno",                           "5bct01"},
    {"Bruno Buccellati",                "5bct01"},
    {"Buccellati",                      "5bct01"},
    {"Diavolo",                         "5dvl01"},
    {"Pannacotta Fugo",                 "5fgo01"},
    {"Fugo",                            "5fgo01"},
    {"Pannacotta",                      "5fgo01"},
    {"Ghiaccio",                        "5gac01"},
    {"Giorno Giovanna",                 "5grn01"},
    {"Giorno",                          "5grn01"},
    {"Giovanna",                        "5grn01"},
    {"Guido Mista",                     "5mst01"},
    {"Mista",                           "5mst01"},
    {"Guido",                           "5mst01"},
    {"Narancia Ghirga",                 "5nrc01"},
    {"Narancia",                        "5nrc01"},
    {"Ghirga",                          "5nrc01"},
    {"Prosciutto & Pesci",              "5prs01"},
    {"Prosciutto",                      "5prs01"},
    {"Pesci",                           "5prs01"},
    {"Risotto Nero",                    "5ris01"},
    {"Risotto",                         "5ris01"},
    {"Trish Una",                       "5trs01"},
    {"Trish",                           "5trs01"},
    // Part 6: Stone Ocean
    {"Narciso Anasui",                  "6ans01"},
    {"Anasui",                          "6ans01"},
    {"Narciso",                         "6ans01"},
    {"Ermes Costello",                  "6elm01"},
    {"Ermes",                           "6elm01"},
    {"Foo Fighters",                    "6fit01"},
    {"F.F.",                            "6fit01"},
    {"FF",                              "6fit01"},
    {"Jolyne Cujoh",                    "6jln01"},
    {"Jolyne",                          "6jln01"},
    {"Jolyne Kujo",                     "6jln01"},
    {"Enrico Pucci (Final)",            "6pci01"},
    {"Enrico Pucci Final",              "6pci01"},
    {"Pucci (Final)",                   "6pci01"},
    {"Pucci Final",                     "6pci01"},
    {"Father Pucci (Final)",            "6pci01"},
    {"Father Pucci Final",              "6pci01"},
    {"Enrico Pucci",                    "6pci02"},
    {"Enrico Pucci (Whitesnake)",       "6pci02"},
    {"Enrico Pucci Whitesnake",         "6pci02"},
    {"Father Pucci",                    "6pci02"},
    {"Father Pucci(Whitesnake)",        "6pci02"},
    {"Father Pucci Whitesnake",         "6pci02"},
    {"Pucci",                           "6pci02"},
    {"Pucci (Whitesnake)",              "6pci02"},
    {"Pucci Whitesnake",                "6pci02"},
    {"Weather Report",                  "6wet01"},
    {"Weather",                         "6wet01"},
    // Part 7: Steel Ball Run
    {"Diego Brando",                    "7dio01"},
    {"Diego",                           "7dio01"},
    {"Scary Monsters Diego Brando",     "7dio01"},
    {"Scary Monsters Diego",            "7dio01"},
    {"Parallel World Diego Brando",     "7dio02"},
    {"Parallel World Diego",            "7dio02"},
    {"Alternate World Diego Brando",    "7dio02"},
    {"Alternate World Diego",           "7dio02"},
    {"Diego THE WORLD",                 "7dio02"},
    {"Diego (THE WORLD)",               "7dio02"},
    {"Diego Brando THE WORLD",          "7dio02"},
    {"Diego Brando (THE WORLD)",        "7dio02"},
    {"THE WORLD Diego Brando",          "7dio02"},
    {"THE WORLD Diego",                 "7dio02"},
    {"Gyro Zeppeli",                    "7jir01"},
    {"Gyro",                            "7jir01"},
    {"Johnny Joestar",                  "7jny01"},
    {"Johnny",                          "7jny01"},
    {"Funny Valentine",                 "7vtn01"},
    {"Valentine",                       "7vtn01"},
    {"Funny",                           "7vtn01"},
    // Part 8: JoJolion
    {"Josuke Higashikata (JoJolion)",   "8jsk01"},
    {"Josuke Higashikata 8",            "8jsk01"},
    {"Josuke (JoJolion)",               "8jsk01"},
    {"Josuke 8",                        "8jsk01"},
    {"Josuk8",                          "8jsk01"},
    {"Gappy",                           "8jsk01"},
    {"Toru",                            "8wou01"},
    {"Tooru",                           "8wou01"},
    {"Wonder of U",                     "8wou01"}
};

std::unordered_map<std::string, int> character_index_reflist = {
    {"Baoh",                            35},
    {"Ikuro Hashizawa",                 35},
    {"Ikuro",                           35},
    {"Hashizawa",                       35},
    // Part 1: Phantom Blood
    {"Dio Brando",                      5},
    {"Dio",                             5},
    {"Dio Brando (Part 1)",             5},
    {"Dio (Part 1)",                    5},
    {"Dio Brando 1",                    5},
    {"Dio 1",                           5},
    {"Jonathan Joestar",                17},
    {"Jonathan",                        17},
    {"Robert E. O. Speedwagon",         84},
    {"Speedwagon",                      84},
    {"REO Speedwagon",                  84},
    {"R.E.O. Speedwagon",               84},
    {"Robert Speedwagon",               84},
    {"Will A. Zeppeli",                 7},
    {"Will A Zeppeli",                  7},
    {"Will Anthonio Zeppeli",           7},
    {"William Anthonio Zeppeli",        7},
    {"Will Antonio Zeppeli",            7},
    {"William Antonio Zeppeli",         7},
    {"William A. Zeppeli",              7},
    {"William A Zeppeli",               7},
    {"Baron Zeppeli",                   7},
    {"Zeppeli",                         7},
    // Part 2: Battle Tendency
    {"Caesar Anthonio Zeppeli",         9},
    {"Caesar Antonio Zeppeli",          9},
    {"Caesar Zeppeli",                  9},
    {"Caesar",                          9},
    {"Caesar A Zeppeli",                9},
    {"Caesar A. Zeppeli",               9},
    {"Esidisi",                         18},
    {"Joseph Joestar",                  3},
    {"Joseph",                          3},
    {"Joseph Joestar (Part 2)",         3},
    {"Joseph (Part 2)",                 3},
    {"Joseph Joestar 2",                3},
    {"Joseph 2",                        3},
    {"2seph",                           3},
    {"Lisa Lisa (Elizabeth)",           15},
    {"Lisa Lisa",                       15},
    {"Elizabeth Joestar",               15},
    {"Elizabeth",                       15},
    {"Kars",                            13},
    {"Ultimate Kars",                   14},
    {"Rudol von Stroheim",              99},
    {"Stroheim",                        99},
    {"Von Stroheim",                    99},
    {"Wamuu",                           6},
    // Part 3: Stardust Crusaders
    {"Muhammad Avdol",                  19},
    {"Mohammed Avdol",                  19},
    {"Muhammad Abdul",                  19},
    {"Mohammed Abdul",                  19},
    {"Avdol",                           19},
    {"Abdul",                           19},
    {"DIO",                             12},
    {"Dio Brando (Part 3)",             12},
    {"Dio (Part 3)",                    12},
    {"Dio Brando 3",                    12},
    {"Dio 3",                           12},
    {"Hol Horse",                       24},
    {"Iggy",                            36},
    {"Old Joseph Joestar",              25},
    {"Old Joseph",                      25},
    {"Oldseph",                         25},
    {"Joseph Joestar (Part 3)",         1},
    {"Joseph (Part 3)",                 1},
    {"Joseph Joestar 3",                1},
    {"Joseph 3",                        1},
    {"3seph",                           1},
    {"Jotaro Kujo",                     1},
    {"Jotaro",                          1},
    {"Jotaro Kujo (Part 3)",            1},
    {"Jotaro (Part 3)",                 1},
    {"Jotaro Kujo 3",                   1},
    {"Jotaro 3",                        1},
    {"3taro",                           1},
    {"Noriaki Kakyoin",                 16},
    {"Kakyoin",                         16},
    {"Noriaki",                         16},
    {"Mariah",                          87},
    {"Jean Pierre Polnareff",           2},
    {"Jean-Pierre Polnareff",           2},
    {"Polnareff",                       2},
    {"Jean Pierre",                     2},
    {"Jean-Pierre",                     2},
    {"Pet Shop",                        92},
    {"Vanilla Ice",                     28},
    // Part 4: Diamond is Unbreakable
    {"Yuya Fungami",                    107},
    {"Yuya",                            107},
    {"Fungami",                         107},
    {"Josuke Higashikata",              29},
    {"Josuke",                          29},
    {"Josuke Higashikata 4",            29},
    {"Josuke 4",                        29},
    {"4suke",                           29},
    {"Jotaro Kujo (Part 4)",            75},
    {"Jotaro Kujo 4",                   75},
    {"Jotaro (Part 4)",                 75},
    {"Jotaro 4",                        75},
    {"4taro",                           75},
    {"Keicho Nijimura",                 100},
    {"Keicho",                          100},
    {"Yoshikage Kira",                  40},
    {"Kira",                            40},
    {"Yoshikage",                       40},
    {"Koichi Hirose",                   22},
    {"Koichi",                          22},
    {"Kosaku Kawajiri",                 41},
    {"Kosaku",                          41},
    {"Okuyasu Nijimura",                10},
    {"Okuyasu",                         10},
    {"Akira Otoishi",                   30},
    {"Akira",                           30},
    {"Otoishi",                         30},
    {"Rohan Kishibe",                   11},
    {"Rohan",                           11},
    {"Kishibe",                         11},
    {"Shigekiyo Yangu",                 31},
    {"Shigekiyo",                       31},
    {"Shigechi",                        31},
    {"Yukako Yamagishi",                81},
    {"Yukako",                          81},
    {"Yamagishi",                       81},
    // Parte 5: Vento Aureo
    {"Leone Abbacchio",                 105},
    {"Abbacchio",                       105},
    {"Leone",                           105},
    {"Bruno Bucciarati",                37},
    {"Bucciarati",                      37},
    {"Bruno",                           37},
    {"Bruno Buccellati",                37},
    {"Buccellati",                      37},
    {"Diavolo",                         32},
    {"Pannacotta Fugo",                 27},
    {"Fugo",                            27},
    {"Pannacotta",                      27},
    {"Ghiaccio",                        96},
    {"Giorno Giovanna",                 20},
    {"Giorno",                          20},
    {"Giovanna",                        20},
    {"Gold Experience Requiem",         21},
    {"GER",                             21},
    {"Guido Mista",                     8},
    {"Mista",                           8},
    {"Guido",                           8},
    {"Narancia Ghirga",                 33},
    {"Narancia",                        33},
    {"Ghirga",                          33},
    {"Prosciutto & Pesci",              93},
    {"Prosciutto",                      93},
    {"Pesci",                           93},
    {"Risotto Nero",                    101},
    {"Risotto",                         101},
    {"Trish Una",                       77},
    {"Trish",                           77},
    // Part 6: Stone Ocean
    {"Narciso Anasui",                  38},
    {"Anasui",                          38},
    {"Narciso",                         38},
    {"Ermes Costello",                  23},
    {"Ermes",                           23},
    {"Foo Fighters",                    97},
    {"F.F.",                            97},
    {"FF",                              97},
    {"Jolyne Cujoh",                    26},
    {"Jolyne",                          26},
    {"Jolyne Kujo",                     26},
    {"Enrico Pucci (Final)",            43},
    {"Enrico Pucci Final",              43},
    {"Pucci (Final)",                   43},
    {"Pucci Final",                     43},
    {"Father Pucci (Final)",            43},
    {"Father Pucci Final",              43},
    {"Enrico Pucci",                    82},
    {"Enrico Pucci (Whitesnake)",       82},
    {"Enrico Pucci Whitesnake",         82},
    {"Father Pucci",                    82},
    {"Father Pucci(Whitesnake)",        82},
    {"Father Pucci Whitesnake",         82},
    {"Pucci",                           82},
    {"Pucci (Whitesnake)",              82},
    {"Pucci Whitesnake",                82},
    {"Weather Report",                  79},
    {"Weather",                         79},
    // Part 7: Steel Ball Run
    {"Diego Brando",                    88},
    {"Diego",                           88},
    {"Scary Monsters Diego Brando",     88},
    {"Scary Monsters Diego",            88},
    {"Parallel World Diego Brando",     102},
    {"Parallel World Diego",            102},
    {"Alternate World Diego Brando",    102},
    {"Alternate World Diego",           102},
    {"Diego THE WORLD",                 102},
    {"Diego (THE WORLD)",               102},
    {"Diego Brando THE WORLD",          102},
    {"Diego Brando (THE WORLD)",        102},
    {"THE WORLD Diego Brando",          102},
    {"THE WORLD Diego",                 102},
    {"Gyro Zeppeli",                    4},
    {"Gyro",                            4},
    {"Johnny Joestar",                  39},
    {"Johnny",                          39},
    {"Funny Valentine",                 34},
    {"Valentine",                       34},
    {"Funny",                           34},
    // Part 8: JoJolion
    {"Josuke Higashikata (JoJolion)",   42},
    {"Josuke Higashikata 8",            42},
    {"Josuke (JoJolion)",               42},
    {"Josuke 8",                        42},
    {"Josuk8",                          42},
    {"Gappy",                           42},
    {"Toru",                            109},
    {"Tooru",                           109},
    {"Wonder of U",                     109}
};

std::string get_character_id_ref(std::string ref) {
    if (!character_id_reflist.contains(ref)) return ref;
    return character_id_reflist[ref];
}

int get_character_index_ref(std::string ref) {
    if (!character_index_reflist.contains(ref)) return -1;
    return character_index_reflist[ref];
}

std::string convert_character_index(int index) {
    switch(index) {
        case 1:     return "3jtr01";
        case 2:     return "3pln01";
        case 3:     return "2jsp01";
        case 4:     return "7jir01";
        case 5:     return "1dio01";
        case 6:     return "2wmu01";
        case 7:     return "1zpl01";
        case 8:     return "5mst01";
        case 9:     return "2csr01";
        case 10:    return "4oky01";
        case 11:    return "4rhn01";
        case 12:    return "3dio01";
        case 13:    return "2krs01";
        case 14:    return "2krs02";
        case 15:    return "2lsa01";
        case 16:    return "3kki01";
        case 17:    return "1jnt01";
        case 18:    return "2esd01";
        case 19:    return "3abd01";
        case 20:    return "5grn01";
        case 21:    return "5gxr01";
        case 22:    return "4koi01";
        case 23:    return "6elm01";
        case 24:    return "3hhs01";
        case 25:    return "3jsp01";
        case 26:    return "6jln01";
        case 27:    return "5fgo01";
        case 28:    return "3vni01";
        case 29:    return "4jsk01";
        case 30:    return "4oti01";
        case 31:    return "5sgc01";
        case 32:    return "5dvl01";
        case 33:    return "5nrc01";
        case 34:    return "7vtn01";
        case 35:    return "0bao01";
        case 36:    return "3igy01";
        case 37:    return "5bct01";
        case 38:    return "6ans01";
        case 39:    return "7jny01";
        case 40:    return "4kir01";
        case 41:    return "4kwk01";
        case 42:    return "8jsk01";
        case 43:    return "6pci01";
        case 44:    return "3stp01";
        case 45:    return "4hnd01";
        case 46:    return "4hdr01";
        case 47:    return "3heg01";
        case 48:    return "3mgr01";
        case 49:    return "3slc01";
        case 50:    return "3slc03";
        case 51:    return "5gex01";
        case 52:    return "5grn02";
        case 53:    return "4ech03";
        case 54:    return "6kis01";
        case 55:    return "6stf01";
        case 56:    return "5pph01";
        case 57:    return "3crm01";
        case 58:    return "3wld01";
        case 59:    return "4crd01";
        case 60:    return "4rhc01";
        case 61:    return "5kcr01";
        case 62:    return "7dfc01";
        case 63:    return "3fll01";
        case 64:    return "5sfg01";
        case 65:    return "6ddw01";
        case 66:    return "6whs01";
        case 67:    return "6cmo01";
        case 68:    return "6mih01";
        case 69:    return "4klq01";
        case 70:    return "4klq02";
        case 71:    return "8saw01";
        case 72:    return "5ars01";
        case 73:    return "7val01";
        case 74:    return "7sld01";
        case 75:    return "4jtr01";
        case 76:    return "4stp01";
        case 77:    return "5trs01";
        case 78:    return "5spg01";
        case 79:    return "6wet01";
        case 80:    return "6wrp01";
        case 81:    return "4ykk01";
        case 82:    return "6pci02";
        case 83:    return "6whs02";
        case 84:    return "1sdw01";
        case 85:    return "1nkm01";
        case 86:    return "1nkm02";
        case 87:    return "3mra01";
        case 88:    return "7dio01";
        case 89:    return "7blt01";
        case 90:    return "7scm01";
        case 91:    return "7scm02";
        case 92:    return "3psp01";
        case 93:    return "5prs01";
        case 94:    return "5psc01";
        case 95:    return "5gfd01";
        case 96:    return "5gac01";
        case 97:    return "6fit01";
        case 98:    return "5wha01";
        case 99:    return "2shm01";
        case 100:   return "4kch01";
        case 101:   return "5ris01";
        case 102:   return "7dio02";
        case 103:   return "7blt02";
        case 104:   return "7wld01";
        case 105:   return "5abc01";
        case 106:   return "5mdb01";
        case 107:   return "4fgm01";
        case 108:   return "4hws01";
        case 109:   return "8wou01";
        case 110:   return "8tor01";
    }
    return std::to_string(index);
}

    } // EOH
} // nucc