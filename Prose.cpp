#include "pch.h"
#include "AWindowsLib.h"

ProseTextItem::ProseTextItem()
{
 Key.Clear();
}

ProseTextItem::ProseTextItem(String key, TextItemType ttype)
{
 ItemType = ttype;
 Key = key;
}

ProseTextItem::~ProseTextItem()
{
}

/////////////////////////////////////////////////

ProseLanguage::ProseLanguage()
{
 ID=0;
}

ProseLanguage::ProseLanguage(int id, String const &lang)
{
 ID=id;
 Name=lang;
 Orientation = LanguageOrientation::Left; 
}

ProseLanguage::ProseLanguage(int id, ProseLanguage::LanguageOrientation orient, String const &name) 
{ 
 ID=id;
 Name=name;
 Orientation = orient;
}

/////////////////////////////////////////////////

ProseUnit::ProseUnit()
{
}

ProseUnit::~ProseUnit()
{
}

bool ProseUnit::OpenProse()
{
 String path;
 bool ret;
 
 Clear();

 path.Clear();
 ret = OpenProse(path, true);
 if (ret == false)
   Clear();

 return ret;
}

bool ProseUnit::OpenProse(String const &path)
{
 bool ret;

 ret = OpenProse(path, false);
 if (ret == false)
   Clear();

 return ret;
}

bool ProseUnit::OpenProse(String const &path, bool isRunTime)
{
 AFileIn file;
 AFileStr verify;
 AFileInt version;
 String filePath;
 bool ret;
 
 Clear();

 m_ProsePath = path;
 
 filePath = path;
 if (filePath.Length() > 0)
  {
   if (filePath.Substring(filePath.Length()-1) != L"\\")
     filePath += L"\\";
  }

 filePath += L"ProseData.dat"; 

 Languages.clear();
 Keys.clear();
 Items.clear();

 if (file.Open(filePath) == false)
  {
   m_Error = file.Error();
   return false;
  }

 verify = file.ReadString();
 if (verify.Success == false)
  {
   m_Error = path;
   m_Error += L" did not contain verification string";
   file.Close();
   return false;
  }

 if (verify.Value != L"ProseDataFileVersion")
  {
   m_Error = path;
   m_Error += L" did not contain verification string";
   file.Close();
   return false;
  }

 version = file.ReadInteger();
 if (version.Success == false)
  {
   m_Error = path;
   m_Error += L" did not contain version number";
   file.Close();
   return false;
  }

 switch(version.Value)
  {
   case 1: 
    {
     ret = LoadV1(file, isRunTime);
    } break;
   default:
    {
     m_Error = path;
     m_Error += L" Version number not defined, upgrade AWindowsLib";
     ret = false;
    }
  }

 file.Close();
 return ret;
}

bool ProseUnit::LoadV1(AFileIn &file, bool isRunTime)
{
 ProseLanguage::LanguageOrientation orient;
 ProseTextItem::TextItemType itemType;
 ProseTextItem item;
 AFileInt langCount, itemCount, subCount;
 AFileInt id, langKey;
 AFileInt iType;
 AFileStr str, key;
 int i,k;

 langCount = file.ReadInteger(); // number of languages
 if (langCount.Success == false)
  {
   m_Error = m_ProsePath;
   m_Error += L" Missing Language count";
   return false;
  }

 if (langCount.Value== 0)
  {
   m_Error = m_ProsePath;
   m_Error += L" Missing English Language";
   return false;
  }

 for (i=0; i<langCount.Value; i++)
  {
   id  = file.ReadInteger();
   if (id.Success == false)
    {
     m_Error = m_ProsePath;
     m_Error += L" Failed to read language id";
     return false;
    }
   iType = file.ReadInteger();
   if (iType.Success == false)
    {
     m_Error = m_ProsePath;
     m_Error += L" Failed to language orientation";
     return false;
    }
   str = file.ReadString();
   if (str.Success == false)
    {
     m_Error = m_ProsePath;
     m_Error += L" Failed to read language name";
     return false;     
    }
   orient = (ProseLanguage::LanguageOrientation)iType.Value;
   Languages.insert(std::pair<int, ProseLanguage>(id.Value, ProseLanguage(id.Value, orient, str.Value))); 
  } 

 itemCount = file.ReadInteger(); // number of items
 if (itemCount.Success == false)
  {
   m_Error = m_ProsePath;
   m_Error += L" Failed to read number of items";
   return false;       
  }

 for (i=0; i<itemCount.Value; i++)
  {
   key = file.ReadString();   // text key of text item
   if (key.Success == false)
    {
     m_Error = m_ProsePath;
     m_Error += L" Failed to read item key";
     return false;     
    }
   iType = file.ReadInteger();  // type of text item
   if (iType.Success == false)
    {
     m_Error = m_ProsePath;
     m_Error += L" Failed to read item type indicator";
     return false;     
    }
   subCount = file.ReadInteger(); // number of specific language texts
   if (subCount.Success == false)
    {
     m_Error = m_ProsePath;
     m_Error += L" Failed to read item text counts";
     return false;     
    }
   if (subCount.Value != langCount.Value && isRunTime == true)
    {
     m_Error = L"Item '";
     m_Error += key.Value;
     m_Error += L"' sub item count does not match language count. ";
     m_Error += L"It is missing an entry";
     file.Close();
     return false; 
    }
   itemType = (ProseTextItem::TextItemType)iType.Value;
   item = ProseTextItem(key.Value, itemType);
   for(k=0; k<subCount.Value; k++)
    {
     langKey = file.ReadInteger(); // language key of entry
     if (langKey.Success == false)
      {
       m_Error = m_ProsePath;
       m_Error += L" Failed to read language key";
       return false;     
      }
     str = file.ReadString(); // text entry
     if (str.Success == false)
      {
       m_Error = m_ProsePath;
       m_Error += L" Failed to read text entry";
       return false;     
      }
     item.LanguageText.insert(std::pair<int, String>(langKey.Value, str.Value));
    }
   Items.insert(std::pair<String, ProseTextItem>(key.Value, item));
   Keys.insert(std::pair<String, int>(item.Key, i));
   ReverseKeys.insert(std::pair<int, String>(i, item.Key));
  }
 return true;
}


bool ProseUnit::Save()
{
 AFileOut file;
 AFileTextOut text;
 String path, line;
 int i;
 
 path = m_ProsePath;
 path += L"\\ProseData.dat"; 

 if (file.Open(path) == false)
  {
   m_Error = file.Error();
   return false;
  }

 file.Write(L"ProseDataFileVersion");
 file.Write(1); // version 1
 
 // languages

 file.Write((int)Languages.size());

 for(const auto &lang : Languages)
  {
   file.Write(lang.second.ID);
   file.Write((int)lang.second.Orientation);
   file.Write(lang.second.Name);
  }

 // items and keys

 file.Write((int)Items.size());

 i = 100;
  
 for(const auto &item : Items)
  {
   file.Write(item.second.Key);
   file.Write((int)item.second.ItemType);
   file.Write((int)item.second.LanguageText.size());
   for(const auto &texts : item.second.LanguageText)
    {
     file.Write(texts.first);  // language id
     file.Write(texts.second); // text
    }
   Keys.insert(std::pair<String, int>(item.second.Key, i++));
  }
 file.Close();

 path = m_ProsePath;
 path += L"\\ProseKeys.h"; 

 if (text.Open(path) == false)
  {
   m_Error = text.Error();
   return false;
  }

 text.Write(L"// Prose Multi-Language Key Definition Header File");
 text.Write();
 text.Write(L"// Add task to copy runtime file ProseData.dat to Debug and Release directories");
 text.Write(L"// Call static function ProseRuntimeClass::LoadReverseKeys() prior to obtaining a text item string");
 text.Write();

 // I guess I could reverse engineer resource.h to obtain unique prose IDs

 text.Write();
 text.Write(L"// If any Prose IDs collide with a Resource ID change 40000");
 text.Write();
 text.Write(L"#define PROSE_ID_START_NUMBER 40000");
 text.Write();
 i = 1;
 for(const auto &item : Items) // 2nd pass write unknown prose text items
  {
   if (item.second.ItemType == ProseTextItem::TextItemType::ProseID)
    {
     line = L"#define ";  
     line += item.second.Key;
     line += L" PROSE_ID_START_NUMBER+";
     line += String::Decimal(i++);
     text.Write(line); 
    }
  }
 text.Write();
 text.Write();
 text.Write(L"class ProseRuntimeClass");
 text.Write(L"{");
 text.Write(L" public:");
 text.Write(L" static void LoadReverseKeys(ProseUnit &prose)");
 text.Write(L"  {");
 text.Write(L"   prose.ReverseKeys.clear();");
 text.Write();
 text.Write(L"   // Load reverse lookup key values");
 text.Write();

 for(const auto &item : Items) // 1st pass write all hard coded resource id numbers 
  {
   if (item.second.ItemType == ProseTextItem::TextItemType::ResourceID)
    {
     line = L"   prose.ReverseKeys.insert(std::pair<int, String>(";
     line += item.second.Key;
     line += L", L\"";
     line += item.second.Key;
     line += L"\"));";
     text.Write(line);
    }
  }

 for(const auto &item : Items) // 2nd pass write unknown prose text items
  {
   if (item.second.ItemType == ProseTextItem::TextItemType::ProseID)
    {
     line = L"   prose.ReverseKeys.insert(std::pair<int, String>(";
     line += item.second.Key;
     line += L", L\"";
     line += item.second.Key;
     line += L"\"));";
     text.Write(line);
    }
  }

 text.Write(L"  }");
 text.Write(L"};");
 text.Close();

 return true;
}

void ProseUnit::Clear()
{
 Languages.clear();
 Languages.insert(std::pair<int, ProseLanguage>(1, ProseLanguage(1, ProseLanguage::LanguageOrientation::Left, L"English")));
 Keys.clear();
 Items.clear();
 m_ProsePath = L"";
 m_Error = L"";
}

void ProseUnit::SetLanguage(ProseLanguage const &lang)
{
 if (lang.ID == 0)
   throw L"language id is 0";
 m_LanguageTarget = lang;
}

String ProseUnit::Text(int id)
{
 ProseTextItem item;
 String key, text;
 
 if (m_LanguageTarget.ID == 0) throw L"Use Prose::SetLanguage(lang) to set target language";

 #ifdef _DEBUG
 if (ReverseKeys.count(id) == 0) throw L"Prose item id not found";
 #endif

 key = ReverseKeys[id];

 #ifdef _DEBUG
 if (Items.count(key) == 0) throw L"ProseTextItem key not found in Items";
 #endif 

 item = Items[key];

 #ifdef _DEBUG
 if (item.LanguageText.count(m_LanguageTarget.ID) == 0) throw L"Item missing entry for current language";
 #endif

 text = item.LanguageText[m_LanguageTarget.ID];

 return text;
}

String ProseUnit::TextArgs(int id, String const &t1, String const &t2, String const &t3, String const &t4)
{
 String text, out;
 std::vector<String> parts;
 std::vector<String> args;
 String pm;
 bool gathering;
 wchar_t chr;
 int i, pmi;

 args.push_back(t1);
 args.push_back(t2);
 args.push_back(t3);
 args.push_back(t4);

 text = ProseUnit::Text(id);  // example: The action @1 failed on item @2 because of reason @3.
 out.Clear();                 // paramters can be in any order, the #s match what was passed in the strings
 gathering = false;

 for (i=0; i<text.Length(); i++)
  {
   chr = text[i];
   if (gathering == true)
    {
     if (chr >= L'0' && chr <= L'9')
       pm += chr;  // probably never be double digits
     else
      {
       gathering = false;
       if (String::TryIntParse(pm, &pmi) == true)
        {
         if (pmi > 0 && pmi <= args.size())
           out += args[pmi - 1];
        }
       out += chr; // append the chr that was not a #
      }
    }
   else
    {
     if ( chr != L'@')  // start of parameter ?
       out += chr;
     else
       gathering = true; // yes, gather the parameter number
    }
  }
 return out;
}