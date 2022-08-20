#pragma once

class ProseTextItem
{
 public:
 enum class TextItemType : int { ResourceID, ProseID };
 ProseTextItem();
 ProseTextItem(String id, TextItemType ttype);
~ProseTextItem();

 String Key;
 TextItemType ItemType;
 std::map<int, String> LanguageText; // language Id & text item

};

class ProseLanguage
{
 public:
 enum class LanguageOrientation { Left, Right };

 ProseLanguage();
 ProseLanguage(int id, String const &name);
 ProseLanguage(int id, LanguageOrientation orient, String const &name);

 int    ID;
 String Name;
 LanguageOrientation Orientation;
};


class ProseUnit
{
 public:

 ProseUnit();
~ProseUnit();

 String ProjectPath() { return m_ProsePath; }
 void SetProjectPath(String const &projPath) { m_ProsePath = projPath; }

 bool OpenProse();                    // runtime version
 bool OpenProse(String const &path);  // editing version

 bool Save();
 void Clear();

 void SetLanguage(ProseLanguage const &lang);
 String Text(int id);
 String TextArgs(int id, String const &t1) { return TextArgs(id, t1, L"", L"", L""); }
 String TextArgs(int id, String const &t1, String const &t2) { return TextArgs(id, t1, t2, L""); }
 String TextArgs(int id, String const &t1, String const &t2, String const &t3) { return TextArgs(id, t1, t2, t3, L""); }
 String TextArgs(int id, String const &t1, String const &t2, String const &t3, String const &t4);

 String Error() { return m_Error; }
 
 ProseLanguage Language;

 std::map<int, ProseLanguage>    Languages;
 std::map<String, int>           Keys;
 std::map<int, String>           ReverseKeys;
 std::map<String, ProseTextItem> Items;

 protected:

 void LoadReverseKeys(); // calls generated code in ProseKeys.h if in runtime mode

 bool OpenProse(String const &path, bool isRunTime);  
 bool LoadV1(AFileIn &file, bool isRunTime);  // version 1

 String m_ProsePath;
 String m_Error;

 ProseLanguage m_LanguageTarget;
 
};

