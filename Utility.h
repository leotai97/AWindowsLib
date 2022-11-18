#pragma once

class String
{
 public:
 
String()
{
 m_Str.clear();
}

String(std::wstring const &s)
{
 m_Str = s;
}

String(String const &s)
{
 m_Str = s.m_Str;
}

String(wchar_t const *s)
{
 m_Str = s;
}

void Clear() 
{ 
 m_Str.clear(); 
}

static String FromStdWstring(std::wstring const &str)
{
 String ret(str);
 return ret;
}

int Length() const 
{ 
 return (int)m_Str.size(); 
}

wchar_t *Chars() const
{
 return (wchar_t *)m_Str.c_str();
}

static int Compare(String const &s1, String const &s2)  // case "insensitive" comparison
{
 int c;

 if (s1.Length() == 0 && s2.Length() == 0)
   return 0;

 if (s1.Length()==0) return -1;
 if (s2.Length()==0) return 1; 

 c = _wcsicmp(s1.Chars(), s2.Chars());

 if (c>0) return 1;
 if (c<0) return -1;
 return 0; 
}

static int Compare(std::wstring const &s1, std::wstring const &s2)
{
 int c;

 if (s1.size() == 0 && s2.size() == 0)
   return 0;

 if (s1.size()==0) return -1;
 if (s2.size()==0) return 1; 

 c = _wcsicmp(s1.c_str(), s2.c_str());

 if (c>0) return 1;
 if (c<0) return -1;
 return 0; 
}

static String StrDup(String const &str, int count)
{
 String out;

 for(int i = 0; i<count; i++)
   out += str;

 return out;
}

static String StrDup(wchar_t chr, int count)
{
 String out;

 for(int i = 0; i<count; i++)
   out += chr;

 return out;
}

static String Decimal(int value, int places)
{
 String ws, pl;
 wchar_t buffer[100];

 ws=L"%0";
 ws+=Decimal(places);
 ws+=L'd';

 ::swprintf_s(buffer, 100, ws.Chars(), value);
 ws=buffer;

 return ws;
}

static String Hexadecimal(int value, int places)
{
 String ws, pl;
 wchar_t buffer[100];

 ws=L"0x%0";
 ws+=Decimal(places);
 ws+=L"x";

 ::swprintf_s(buffer, 100, ws.Chars(), value);
 ws=buffer;

 return ws;
}

static String Decimal(int value)
{
 String ws;
 wchar_t buffer[100];

 ::swprintf_s(buffer, 100, L"%d", value);
 ws=buffer;

 return ws;
}

static String Double(double value)
{
 String ws;
 wchar_t buffer[100];

 ::swprintf_s(buffer, 100, L"%f", value);
 ws=buffer;

 return ws;
}

static String Double(double value, int places, int decimals)
{
 String arg;
 String ws;
 wchar_t buffer[100];

 arg = L"%";
 arg += Decimal(places);
 arg + L".";
 arg += Decimal(decimals);
 arg += L"f";

 ::swprintf_s(buffer, 100, arg.Chars(), value);
 ws=buffer;

 return ws;
}

static String GetLastErrorMsg(DWORD val)
{
 String ws;
 wchar_t err[256];
 memset(err, 0, 256);

 ::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, val, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), err, 255, NULL);
 ws=err;
 return ws;
}

static String LoadString(HINSTANCE hInst, UINT id)
{
 String ret;
 wchar_t *szText;

 szText=new wchar_t[1000];
 ::LoadString(hInst, id, szText, 1000);
 ret = szText;
 delete [] szText;
 return ret;
}

static bool TryIntParse(String const &iVal, int *retVal)
{
 String check;
 String val;
 int v;
 int i;  

 check = L"0123456789";

 val = iVal.Trim();  // trim spaces

 for (i=0;i<val.Length();i++)  // have to manually check for invalid chrs in the number
  {
   if (check.IndexOf(val[i]) == -1)
    {
     if (i > 0) 
       return false; // found a non numeric chr beyond the 1st
     else
      {
       if (val[i] != L'-') 
         return false;  // only #s and negative sign allowed in 1st position 
      } 
    }
  }

 try
  {
   v=std::stoi(val.Chars(), nullptr, 10);  // atoi will work as long as start of string is numeric
   *retVal=v;
   return true;
  }
 catch(...)
  {
   return false;
  }
}


static bool TryDblParse(String const &val, double *retVal)
{
 double v;

 try
  {
   v=std::stod(val.Chars(), nullptr);
   *retVal=v;
   return true;
  }
 catch(...)
  {
   return false;
  }
}

static String Repeat(String const &val, int times)
{
 String out;
 int i;

 for (i=0;i<times;i++)
   out+=val;

 return out;
}

int IndexOf(wchar_t chr) const
{
 int i;
 
 if (Length()==0)
   return -1;

 for(i=0;i<Length();i++)
  {
   if (m_Str[i] == chr)
     return i; 
  }
 return -1;
}

int IndexOf(String const &str) const
{
 int i,c;

 if (str.Length()==0)
   return -1;

 if (str.Length() > Length())
   return -1; 

 c=Length() - (str.Length()-1);

 for(i=0;i<c;i++)
  {
   if (Substring(i, str.Length()) == str)
     return i; 
  }
 return -1;
}

int LastIndexOf(wchar_t const chr) const
{
 int i;

 if (Length() == 0)
   return -1;
 
 for(i=Length()-1;i>=0;i--)
  {
   if (m_Str[i] == chr)
     return i; 
  }
 return -1;
}

int LastIndexOf(String const &str) const
{
 int i,c, len;

 if (str.Length()==0)
   return -1;

 if (str.Length() > Length())
   return -1; 

 len = Length();

 c=len - (str.Length());

 for(i=c;i>=0;i--)
  {
   if (Substring(i, str.Length()) == str)
     return i; 
  }
 return -1;
}

int NotIndexOf(wchar_t chr) const
{
 int i,c;

 if (Length() == 0) 
   return 0;

 c=Length();

 for(i=0;i<c;i++)
  {
   if (m_Str[i] != chr)
     return i; 
  }
 return -1; 
}

static std::wstring ToLower(std::wstring const &str)
{
 String v1, v2;
 std::wstring output;

 v1=str;
 v2=v1.ToLower();
 output = v2.Chars();

 return output;
}

static std::wstring ToLower(String const &str)
{
 String v2;
 std::wstring output;

 v2=str.ToLower();
 output = v2.Chars();

 return output;
}
 
String ToLower() const
{
 String ret(m_Str);
 ::CharLower((LPWSTR)ret.Chars()); 
 return ret;
}

String ToUpper() const
{
 String ret(m_Str);
 ::CharUpper((LPWSTR)ret.Chars()); 
 return ret;
}

String Right(int rv) const
{
 String ret;
 
 ret = Substring( Length() - rv, rv);
 return ret;
}

String Substring(int index, int len) const
{
 String ret;

 if (index >= Length())
   throw L"index is beyond string boundry";

 ret=FromStdWstring(m_Str.substr(index, len));
 return ret;
}

String Substring(int index) const
{
 String ret;

 if (index >= Length())
   throw L"index is beyond string boundry";

 ret=FromStdWstring(m_Str.substr(index));
 return ret;
}


std::vector<String> Split(wchar_t const chr) const  // returns array of string, needs to be [] deleted
{
 std::vector<String> list;
 String accum;
 int i;

 accum.Clear();

 for(i=0;i<Length();i++)
  {
   if (m_Str[i]==chr)
    {
     list.push_back(String(accum));
     accum.Clear();
    }
   else
     accum+=m_Str[i];
  }
 if (accum.Length()>0)
   list.push_back(String(accum));
 
 return list;
}

std::vector<String> Split(String const &str) const     // returns array of string, needs to be [] deleted
{
 String ss;
 std::vector<String> list;
 String accum;
 int i;

 if (str.Length() > Length())
   throw L"Length of delimiter > than String";

 accum.Clear();

 for(i=0;i<Length();i++)
  {
   ss=Substring(i, str.Length());
   if (ss == str)
    {
     i+=str.Length()-1;
     list.push_back(String(accum));
     accum.Clear();
    }
   else
     accum+=m_Str[i];
  }
 if (accum.Length()>0)
   list.push_back(String(accum));

 return list;
}


String operator = (String const &s)
{
 m_Str = s.Chars();
 return *this;
}

String operator += (String const &s)
{
 std::wstring rv;

 rv=m_Str;
 rv += s.Chars();
 m_Str=rv;

 return *this;
}

String operator += (std::wstring const &s)
{
 std::wstring rv;

 rv=m_Str;
 rv+=s;
 m_Str=rv;

 return *this;
}
  
String operator += (wchar_t const chr)
{
 std::wstring rv;

 rv=m_Str;
 rv+=chr;
 m_Str=rv;

 return *this;
}

String operator += (wchar_t const *chr)
{
 std::wstring rv;

 rv=m_Str;
 rv+=chr;
 m_Str=rv;

 return *this;
}

String operator + (String const &s)
{
 std::wstring rv;

 rv=m_Str;
 rv += s.Chars();
 m_Str=rv;
 return *this;
}

String operator + (wchar_t const c)
{
 std::wstring rv;

 rv=m_Str;
 rv += c;
 m_Str=rv;
 return *this;
}

bool operator == (String const &str) const
{
 std::wstring compare=str.Chars();

 return compare == m_Str;
}


bool operator == (wchar_t const c) const
{
 if (Length()!=1)
   return false;

 return m_Str[0]==c;
}

bool operator == (wchar_t const *str) const
{
 std::wstring compare=str;

 return compare == m_Str;
}

bool operator != (String const &str) const
{
 std::wstring compare=str.Chars();

 return compare != m_Str;
}

bool operator != (wchar_t const *chr) const
{
 std::wstring compare=chr;

 return compare != m_Str;
}

bool operator != (wchar_t const c) const
{
 if (Length()!=1)
   return true;

 return m_Str[0]!=c;
}

bool operator () (const String &s1, const String &s2) const
{
 return String::Compare(s1,s2) < 0;  // ascending
}

bool operator < (const String &other) const
{
 return String::Compare(*this, other) < 0;
}

wchar_t operator [] (int i) const
{
 if (i<0) throw L"i < 0";
 if (i>=(int)m_Str.size()) throw L"i > string length";

 return m_Str[i];
}


bool Equal(String const &compare) const
{
 return compare.m_Str == m_Str;
}

bool Equal(wchar_t const *str) const
{
 std::wstring compare=str;
 return compare == m_Str;
}

bool Equal(wchar_t const c) const
{
 std::wstring compare;

 compare=c;

 return compare == m_Str;
}

String Trim() const       // returns trim leading and tailing
{
 String ret;

 ret=TrimRight();
 return ret.TrimLeft();
}

String TrimRight() const  // returns strim tailing
{
 String ns;
 int i,ni,c;

 c=(int)m_Str.size();
 ni=-1;
 ns.Clear();

 for(i=c-1; i>=0; i--)
  {
   if (m_Str[i] != L' ')
    {
     ni=i;
     break;
    }
  } 

 if (ni==c-1)
   return m_Str;  // no trailing spaces;

 if (ni == -1)
  {
   return ns;  // was all spaces
  }

 ns=m_Str.substr(0, ni+1);
 return ns;
}

String TrimLeft() const   // returns trim leading
{
 String ns;
 int i,ni,c;

 c=(int)m_Str.size();
 ni=-1;
 ns.Clear();

 for(i=0;i<c;i++)
  {
   if (m_Str[i] != L' ')
    {
     ni=i;
     break;
    }
  } 

 if (ni==0)
   return m_Str;  // no leading spaces;

 if (ni == -1)
  {
   return ns;
  }

 ns=m_Str.substr(ni);
 return ns;
}

std::wstring StdString() 
{ 
 return m_Str; 
}

static void Test()
{
 String t1, t2, t3, t4;
 
 String test(L"testing");

 t1=L"Test";
 t2=L"Test";
 t3=L"abc";
 t4=L"st";

 if (! t1.Equal(t2)) throw L"String() failed";
 if ( String::Compare(t1, t2) != 0) throw L"Compare failed";
 if ( String::Compare(t3, t1) >= 0) throw L"Compare failed";
 if ( String::Compare(t1, t3) <= 0) throw L"Compare failed";
 if ((t1 == t2)==false) throw L"== failed";
 if ((t1 != t2)==true) throw L"!= failed";
 if ((t1 == L"Test")==false) throw L" ==  wchar_t failed";
 if ((t3 == L"abc") == false) throw L"String(wchar) failed";
 if (t1.Length() != 4) throw L"Length failed";
 if (t1.Chars()[1]!= L'e') throw L"Chars failed";
 if (t1.Substring(0,1).Equal(L'T') == false) throw L"Substring failed";
 if (t1.Substring(1,1).Equal(L'e') == false) throw L"Substring failed";
 if (t1.Substring(1).Equal(L"est") == false) throw L"Substring failed";
 if (t1.Substring(1,2).Equal(L"es") == false) throw L"Substring failed";
 if (t1.IndexOf(L's') != 2) throw L"IndexOf failed";
 if (t1.IndexOf(L'z') != -1) throw L"IndexOf failed";
 if (t1.IndexOf(t4) != 2) throw L"IndexOf failed";
 if (t1.LastIndexOf(L'e') != 1) throw L"LastIndexOf failed";
 if (t1.LastIndexOf(t4) != 2) throw L"LastIndexOf failed";
 if (String::Compare(t1.ToUpper(),L"TEST") != 0) throw L"ToUpper failed";
 if (String::Compare(t1.ToUpper(),L"test") != 0) throw L"ToUpper failed";
 if (t1.Right(3) != L"est") throw L"Right failed"; 
 try
  {
   t1.Substring(5,1);
   throw L"bad Substring didn't throw";
  }
 catch(...)
  {
  }

 String t5=L"abc|def|ghi|jkl|mno|pqr|stu|vwy|z";
 String *s=nullptr;

 std::vector<String> list;

 list=t5.Split(L'|');
 if (list.size() != 9) throw L"split return failed"; 
 if (list[0] != L"abc") throw L"split failed";
 if (list[1] != L"def") throw L"split failed";
 if (list[2] != L"ghi") throw L"split failed";
 if (list[3] != L"jkl") throw L"split failed";
 if (list[8] != L"z") throw L"split failed";


 String t6=L"abc..def..ghi..jkl..mno..pqr..stu..vwy..z";
 String t7=L"..";
 list=t6.Split(t7);
 if (list.size() != 9) throw L"split return failed";
 if (list[0] != L"abc") throw L"split failed";
 if (list[1] != L"def") throw L"split failed";
 if (list[2] != L"ghi") throw L"split failed";
 if (list[3] != L"jkl") throw L"split failed";
 if (list[8] != L"z") throw L"split failed";

 t1=String::Decimal(1234);
 if (t1 != L"1234") throw L"Decimal failed";

 t1=String::Decimal(36,4);
 if (t1 != L"0036") throw L"Decimal failed";

 t1=String::Hexadecimal(32,4);
 if (t1 != L"0x0020") throw L"hexadecimal failed";
 
 t1=L"10";
 int v=0;
 double d;
 if (String::TryIntParse(t1, &v)==false || v!=10) throw L"TryIntParse failed";
 if (String::TryDblParse(t1, &d)==false || d!=10.0) throw L"TryIntParse failed";
 
 t1=L"a";
 t2=L"b";
 t3=L"c";
 String f;

 f=t1 + t2 + t3;
 if (f != L"abc") throw L"concat failed"; 

 t1=L"        test";
 t2=t1.TrimLeft();
 if (t2 != L"test") throw L"TrimLeft return failed";

 t1=L"test                    ";
 t2=t1.TrimRight();
 if (t2 != L"test") throw L"TrimRight return failed";

 t1=L"                         te  st                    ";
 t2=t1.Trim();
 if (t2 != L"te  st") throw L"Trim return failed";

 t1=L"test";
 if (t1[1] != L'e') throw L"[] operator failed";

 t2=L"Test";
 if ( ! (t1.ToLower() == t2.ToLower()) ) throw L"To lower comparison failed";
 if ( t1.ToLower() != t2.ToLower() ) throw L"Tolower failed";

}

private:

 std::wstring m_Str;

};

struct // sort Strings
 {
  bool operator()(const String &a, const String &b) const { return String::Compare(a,b) < 0; } // low to high
 } AStringSort;

class Random
{
 public:

 Random()
 {
  std::random_device r;
   
  m_RE = std::default_random_engine(r());
  m_Dist = std::uniform_int_distribution<int>(1, INT_MAX);
 }

 Random(int max) 
 {
  std::random_device r;
    
  m_RE = std::default_random_engine(r());
  m_Dist = std::uniform_int_distribution<int>(1, max);
 }

 int Next()
 {
  return m_Dist(m_RE);
 }  

 private:

 std::default_random_engine m_RE;
 std::uniform_int_distribution<int> m_Dist;
 
};

class RandomString
{
 public:
 RandomString(String const &txt, int rnd)
 {
  Text = txt;
  Random = rnd;
 }

 String Text;
 int    Random;
};

struct RandomStringSort
{
 bool operator()(RandomString const &a, RandomString const &b) const { return a.Random < b.Random; }
};

class DateTime
{
 public:

 enum class Format : int { YMD, MDYHMS, HMSAP, HMS };
 
 DateTime()
 {
  m_Year=0;
  m_Month=0;
  m_Day=0;
  m_Hour=0;
  m_Minute=0;
  m_Second=0;
 }

 DateTime(int y, int m, int d, int h, int mn, int s)
 {
  m_Year=y;
  m_Month=m;
  m_Day=d;
  m_Hour=h;
  m_Minute=mn;
  m_Second=s;
 }

 bool IsValid()
 {
  // if year is divisible by 4 and is not divisible by 100 it's leap 
  // if year divisible by 400 it's leap

  if ( m_Year<1532 )           return false; // start of gregorian calendar
  if ( m_Month<1 || m_Month>12 ) return false;
  if ( m_Day<1 || m_Day>31)      return false;

  if ( m_Month == 2 || m_Month == 4 || m_Month ==6 || m_Month == 9 || m_Month==11)
   {
    if ( m_Day==31 ) return false;
   }
  if (m_Month == 2)
   {
    if ( m_Day == 30 ) return false;
    if ( m_Day == 29 )
     {
      if ( m_Year % 4 == 0 && m_Year % 100 ) return true;
      if ( m_Year % 400 ) return true;
     }
   }
  return true;
 }

 static DateTime Now()
 {
  DateTime date;
  time_t current_time;
  tm     local_time;

  time ( &current_time );
  localtime_s(&local_time, &current_time);

  date.m_Year   = local_time.tm_year + 1900;
  date.m_Month  = local_time.tm_mon + 1;
  date.m_Day    = local_time.tm_mday;

  date.m_Hour   = local_time.tm_hour;
  date.m_Minute = local_time.tm_min;
  date.m_Second = local_time.tm_sec;

  return date;
 }

 static DateTime FromSystemTime(SYSTEMTIME const &st)
 {
  DateTime date;

  date.m_Year   = st.wYear;
  date.m_Month  = st.wMonth;
  date.m_Day    = st.wDay;

  date.m_Hour   = st.wHour;
  date.m_Minute = st.wMinute;
  date.m_Second = st.wYear;

  return date;
 }

 static DateTime MinValue()
 {
  DateTime date;

  date.m_Year=0;
  date.m_Month=0;
  date.m_Day=0;
  date.m_Hour=0;
  date.m_Minute=0;
  date.m_Second=0; 

  return date;
 }

 static String GetMonth(int m)
 {
  String s;

  switch(m)
   {
    case 1: s=L"Jan"; break;
    case 2: s=L"Feb"; break;
    case 3: s=L"Mar"; break;
    case 4: s=L"Apr"; break;
    case 5: s=L"May"; break;
    case 6: s=L"Jun"; break;
    case 7: s=L"Jul"; break;
    case 8: s=L"Aug"; break;
    case 9: s=L"Sep"; break;
    case 10: s=L"Oct"; break;
    case 11: s=L"Nov"; break;
    case 12: s=L"Dec"; break;
   }
  return s;
 }


 String ToString(Format fmt)
 {
  String ret;

  switch(fmt)
   {
    case DateTime::Format::YMD:
     {
       ret =String::Decimal(m_Year,4);
       ret+=L'/';
       ret+=String::Decimal(m_Month,2);
       ret+=L'/';
       ret+=String::Decimal(m_Day,2);
       return ret;
     } 
    case DateTime::Format::MDYHMS:
     {
      ret = GetMonth(m_Month);
      ret+=L" ";
      ret+=String::Decimal(m_Day);
      ret+=L" ";
      ret+=String::Decimal(m_Year, 4);
      ret+=L" ";
      ret+=ToString(DateTime::Format::HMSAP);
      return ret;
     } 
    case DateTime::Format::HMS:
     {
      ret =String::Decimal(m_Hour, 2);
      ret+=L":";
      ret+=String::Decimal(m_Minute,2);
      ret+=L":";
      ret+=String::Decimal(m_Second,2);
      return ret;
     } 
    case DateTime::Format::HMSAP:
     {
      int h;
      if (m_Hour>12) h=m_Hour-12; else h=m_Hour;
      ret =String::Decimal(h, 2);
      ret+=L":";
      ret+=String::Decimal(m_Minute,2);
      ret+=L":";
      ret+=String::Decimal(m_Second,2);
      if (m_Hour>12) ret+=L"PM"; else ret+=L"AM";
      return ret;
     } 
   }
  return ret;
 }

 bool operator == (DateTime const &dt) const
 {
  if (dt.m_Year != m_Year) return false;
  if (dt.m_Month != m_Month) return false;
  if (dt.m_Day != m_Day) return false;
  if (dt.m_Hour != m_Hour) return false;
  if (dt.m_Minute != m_Minute) return false;
  if (dt.m_Second != m_Second) return false;
  return true;
 }

 bool operator != (DateTime const &dt) const
 {
  if (dt.m_Year != m_Year) return true;
  if (dt.m_Month != m_Month) return true;
  if (dt.m_Day != m_Day) return true;
  if (dt.m_Hour != m_Hour) return true;
  if (dt.m_Minute != m_Minute) return true;
  if (dt.m_Second != m_Second) return true;
  return false;
 }

 bool operator > (DateTime const &dt) const
 {
  if (m_Year > dt.m_Year) return true;
  if (m_Year < dt.m_Year) return false;

  if (m_Month > dt.m_Month) return true;
  if (m_Month < dt.m_Month) return false;

  if (m_Day > dt.m_Day) return true;
  if (m_Day < dt.m_Day) return false;

  if (m_Hour > dt.m_Hour) return true;
  if (m_Hour < dt.m_Hour) return false;

  if (m_Minute > dt.m_Minute) return true;
  if (m_Minute < dt.m_Minute) return false;

  if (m_Second > dt.m_Second) return true;

  return false;
 }

 bool operator < (DateTime const &dt) const
 {
  if (m_Year < dt.m_Year) return true;
  if (m_Year > dt.m_Year) return false;

  if (m_Month < dt.m_Month) return true;
  if (m_Month > dt.m_Month) return false;

  if (m_Day < dt.m_Day) return true;
  if (m_Day > dt.m_Day) return false;

  if (m_Hour < dt.m_Hour) return true;
  if (m_Hour > dt.m_Hour) return false;

  if (m_Minute < dt.m_Minute) return true;
  if (m_Minute > dt.m_Minute) return false;

  if (m_Second < dt.m_Second) return true;

  return false;
 }

 bool IsMinValue()
 {
  if (m_Year ==0 && m_Month==0 && m_Day==0) 
    return true;

  return false;
 }

 bool LeapYear() 
 {
  if ( m_Year % 4 == 0 && m_Year % 100 ) return true;
  if ( m_Year % 400 ) return true;

  return false;
 }


 static void Test()
 {
  DateTime d1(2002, 12, 1, 3, 4, 5);
  DateTime d2(2002, 12, 1, 3, 4, 5);
  DateTime d3(2002, 12, 1, 3, 4, 6);
 
  if (d1.ToString(DateTime::Format::YMD) != L"2002/12/01") throw L"Tostring failed";
 
  if (!(d1 == d2)) throw L" == failed, they are equal";
  if ( (d1 == d3)) throw L" == failed, they are not equal";
  if ( (d1 != d2)) throw L" != failed, they are equal";
  if( !(d1 != d3)) throw L" != failed, they are not equal";

  if ( (d1 > d2)) throw L" > failed they are equal";
  if ( (d1 < d2)) throw L" < failed they are equal";

  if ( !(d1 < d3)) throw L" < failed, d1 is < d3";
  if ( !(d3 > d1)) throw L" < failed, d3 is > d3";
 }

 int Year() const { return m_Year; }
 int Month() const { return m_Month; }
 int Day() const { return m_Day; }
 int Hour() const { return m_Hour; }
 int Minute() const { return m_Minute; }
 int Second() const { return m_Second; }

 private:

 int m_Year;
 int m_Month;
 int m_Day;
 int m_Hour;
 int m_Minute;
 int m_Second;
};

class Utility
{
 public:

 static int IntCompare(int x, int y)
  {
   if (x>y) return 1;
   if (x<y) return -1;
   return 0;
  }

 static bool StdIntCompare(int x, int y)
  {
   if (x<y) 
     return true;
   else
     return false;
  }

 static bool ComparePercentage(int v1, int v2, float percent)
 {
  float above;

  above = 1.0F + (1.0F - percent);

  if (v1 == v2) 
    return true;

  if (v1 < (int)((float)v2 * percent)) return false;
  if (v1 > (int)((float)v2 * above))   return false;

  if (v2 < (int)((float)v1 * percent)) return false;
  if (v2 > (int)((float)v1 * above))   return false;

  return true;
 }




 static inline Rect GetRect(RECT const &r) 
 { 
  return Rect(r.left, r.top, r.right - r.left, r.bottom-r.top); 
 }

 static inline Size GetSize(Rect const &r) 
 { 
  Size sz; 
  
  r.GetSize(&sz); return sz; 
 }

 static bool IsDarkThemeActive()
 {
  DWORD   type;
  DWORD   value;
  DWORD   count = 4;

  LSTATUS st = RegGetValue(
        HKEY_CURRENT_USER,
        TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize"),
        TEXT("AppsUseLightTheme"),
        RRF_RT_REG_DWORD,
        &type,
        &value,
        &count );
  if ( st == ERROR_SUCCESS && type == REG_DWORD )
    return value == 0;
  return false;
 }

 static LPARAM AscendDescendColumn( bool ascDesc, int col)
 {
  LPARAM v=0;

  if (ascDesc == true)
    v = 0x0100;
  else
    v = 0x0000;

   v |= col;

  return v;  
 }

 static bool FileExists(String const &path)
 { 
  if ( _taccess_s( path.Chars(), 0 ) == 0 )
   { 
    return true; 
   }
  return false;
 }

 static bool DirectoryExists(String const &path)
 { 
  struct _stat status;

  if ( _taccess_s( path.Chars(), 0 ) == 0 )
   {  
    _tstat( path.Chars(), &status );
    return (status.st_mode & S_IFDIR) != 0;
   }
  return false;
 }

 static String DirectoryCreate(String const &path)
 {
  DWORD err;
  String msg;

  if (::CreateDirectory(path.Chars(), 0) == FALSE)
   {
    err = GetLastError();
    switch(err)
     {
      case ERROR_ALREADY_EXISTS: msg = L"Directory Already Exists: "; break;
      case ERROR_PATH_NOT_FOUND: msg = L"Part of the path not found: "; break;
      default: msg = L"Unknown Error: "; break;
     }
    msg += path;
   }

  return msg;
 }

 static String DirectoryName(String const &pathIn) // extract directory name from file path
 {
  String name, path;
  int i;

  if (pathIn.Length() == 0) 
    return path;

  if (pathIn.Substring(pathIn.Length() - 1, 1) == L"\\")
    path = pathIn.Substring(0, pathIn.Length()-1);
  else
    path = pathIn;

  i = path.LastIndexOf(L"\\");
  if (i < 0)
    return path;

  name = path.Substring(i+1);

  return name;
 }


 static std::vector<String> GetFileNames(String const &dir, String const &filter)
 {
  WIN32_FIND_DATA data;
  HANDLE fr;
  std::vector<String> list;
  String sSearch;
  void *oldVal;

  Wow64DisableWow64FsRedirection(&oldVal);

  if (DirectoryExists(dir) == false)
    return list;
 
  sSearch = dir;
  if (dir.Substring(dir.Length()-1,1) != L"\\")
    sSearch += L"\\";
  sSearch += filter;

  fr = ::FindFirstFile(sSearch.Chars(), &data);
  if (fr == INVALID_HANDLE_VALUE)
   {
    return list;  // empty
   }
  do
   {
    list.push_back(data.cFileName);
   }
  while(::FindNextFile(fr, &data)==TRUE);
  ::FindClose(fr);

  Wow64RevertWow64FsRedirection(&oldVal);

  return list;
 }

 static std::vector<String> GetFolderNames(String const &dir) 
 {
  WIN32_FIND_DATA data;
  HANDLE fr;
  std::vector<String> list;
  String sSearch;
  void *oldVal;

  Wow64DisableWow64FsRedirection(&oldVal);

  if (DirectoryExists(dir) == false)
    return list;

  sSearch = dir;
  if (dir.Substring(dir.Length()-1,1) != L"\\")
    sSearch += L"\\";

  sSearch += L"*";

  fr = ::FindFirstFileEx(sSearch.Chars(), FindExInfoStandard, &data, FindExSearchLimitToDirectories, NULL, 0);
  if (fr == INVALID_HANDLE_VALUE)
   {
    return list;  // empty
   }
  do
   {
    list.push_back(data.cFileName);
   }
  while(::FindNextFile(fr, &data)==TRUE);
  ::FindClose(fr);

  Wow64RevertWow64FsRedirection(&oldVal);

  return list;
 }
  
};

/////////////////////////////////////////////////////////

class AFile
{
 public:

 enum class DataType : BYTE { Text = 1, Integer = 2 };

 AFile()
  {
   m_hFile = 0;
  }

~AFile()
 {
  Close();
 }

 void Close()
 {
  if (m_hFile != 0)
   {
    CloseHandle(m_hFile);
    m_hFile = 0;
   } 
 }

 String Error() { return m_Error; }

 protected:

 String m_Path;
 String m_Error;
 HANDLE m_hFile; 
};

class AFileData
{
 public:

 enum class DataTypes : BYTE { Text = 1, Integer = 2 };

 DataTypes DataType;
 bool Success;

};

class AFileStr : public AFileData
{
 public:
 AFileStr() { DataType = DataTypes::Text; }

 String Value;
};

class AFileInt : public AFileData
{
 public:
 AFileInt() { DataType = DataTypes::Integer; Value=0; }

 int Value;
};

class AFileIn : public AFile
{
 public:

 bool Open(String const &path)
 {
  HANDLE hFile;

  hFile = CreateFile(path.Chars(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);    
  if (hFile == INVALID_HANDLE_VALUE)
   {
    m_Error = String::GetLastErrorMsg(GetLastError());
    return false;
   }
  else
   {
    m_hFile = hFile;
    m_Path = path;
    return true;
   }
 }

 AFileInt ReadInteger()
 {
  AFileInt ret;
  DWORD br;
  BYTE rType;
  BOOL rr;
  int i;

  rr = ReadFile(m_hFile, &rType, 1, &br, NULL);
  if (rr == FALSE)
   {
    m_Error = String::GetLastErrorMsg(GetLastError());
    ret.Success = false;
    return ret;
   }

  if (rType != (BYTE)AFileData::DataTypes::Integer)
   {
    m_Error = L"Data type read from file not integer";
    ret.Success = false;
    return ret;
   }

  rr = ReadFile(m_hFile, &i, 4, &br, NULL); // 4 byte integer
  if (rr == FALSE)
   {
    m_Error = String::GetLastErrorMsg(GetLastError());
    ret.Success = false;
    return ret;
   }

  ret.Value = i;
  ret.Success = true;

  return ret;
 }

 AFileStr ReadString()
 {
  AFileStr ret;
  DWORD br, rLen;
  wchar_t *buffer;
  BYTE rType;
  BOOL rr;
  unsigned short len;

  rr = ReadFile(m_hFile, &rType, 1, &br, NULL);
  if (rr == FALSE)
   {
    m_Error = String::GetLastErrorMsg(GetLastError());
    ret.Success = false;
    return ret;
   }

  if (rType != (BYTE)AFileData::DataTypes::Text)
   {
    m_Error = L"Data type read from file not text";
    ret.Success = false;
    return ret;
   }

  rr = ReadFile(m_hFile, &len, 2, &br, NULL); // 2 byte string length
  if (rr == FALSE)
   {
    m_Error = String::GetLastErrorMsg(GetLastError());
    ret.Success = false;
    return ret;
   }

  if (len == 0)
   {
    ret.Value.Clear();
    ret.Success = true;
    return ret;
   }

  buffer = new wchar_t[len + 1]; 
  rLen = len * sizeof(wchar_t);
  rr = ReadFile(m_hFile, buffer, rLen, &br, NULL); // read wchar_t 
  if (rr == FALSE)
   {
    m_Error = String::GetLastErrorMsg(GetLastError());
    ret.Success = false;
    return ret;
   }

  buffer[len]=L'\0'; // terminate string
 
  ret.Value = buffer;
  ret.Success = true;
 
  delete [] buffer;
 
  return ret;
 }

};

class AFileOut : public AFile
{
 public:

 bool Open(String const &path)
 {
  HANDLE hFile;

  hFile = CreateFile(path.Chars(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);    
  if (hFile == INVALID_HANDLE_VALUE)
   {
    m_Error = String::GetLastErrorMsg(GetLastError());
    return false;
   }
  else
   {
    m_hFile = hFile;
    m_Path = path;
    return true;
   }
 }

 void Write(int val)
 {
  BOOL wr;
  DWORD bw;
  BYTE dt;

  if (m_hFile == 0)
    throw L"file not open";

  dt = (BYTE)AFileData::DataTypes::Integer;

  wr = WriteFile(m_hFile, &dt, 1, &bw, NULL);
  if (wr == FALSE)
   {
    m_Error = String::GetLastErrorMsg(GetLastError());
    throw L"out of disk space?";
   }

  wr = WriteFile(m_hFile, &val, 4, &bw, NULL);
  if (wr == FALSE)
   {
    m_Error = String::GetLastErrorMsg(GetLastError());
    throw L"out of disk space?";
   }
 }

 void Write(String const &val)
 {
  BOOL wr;
  DWORD bw, rLen;
  BYTE dt;
  unsigned short len;

  if (m_hFile == 0)
    throw L"file not open";

  dt = (BYTE)AFileData::DataTypes::Text;

  wr = WriteFile(m_hFile, &dt, 1, &bw, NULL);
  if (wr == FALSE)
   {
    m_Error = String::GetLastErrorMsg(GetLastError());
    throw L"out of disk space?";
   }

  len = (unsigned short)val.Length();
  wr = WriteFile(m_hFile, &len, 2, &bw, NULL);
  if (wr == FALSE)
   {
    m_Error = String::GetLastErrorMsg(GetLastError());
    throw L"out of disk space?";
   }

  rLen = len * sizeof(wchar_t);
  wr = WriteFile(m_hFile, val.Chars(), rLen, &bw, NULL);
  if (wr == FALSE)
   {
    m_Error = String::GetLastErrorMsg(GetLastError());
    throw L"out of disk space?";
   }
 }

};

class AFileTextOut : public AFile
{
 public:

 bool Open(String const &path) 
 {
  HANDLE hFile;

  hFile = CreateFile(path.Chars(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);    
  if (hFile == INVALID_HANDLE_VALUE)
   {
    m_Error = String::GetLastErrorMsg(GetLastError());
    return false;
   }
  else
   {
    m_hFile = hFile;
    m_Path = path;
    m_FirstLine = true;
    return true;
   }
 }

 void Write() 
  { 
   Write(L""); 
  }

 void Write(String const &line) 
 {
  BOOL wr;
  DWORD bw;
  char *buffer;
  int i, len;

  if (m_hFile == 0)
    throw L"file not open";

  if (m_FirstLine == false) 
   {
    wr = WriteFile(m_hFile, "\r\n", 2, &bw, NULL);
    if (wr == FALSE)
     {
      m_Error = String::GetLastErrorMsg(GetLastError());
      throw L"out of disk space?";
     }
   }
  else
   {
    m_FirstLine = false;
   }

  len = line.Length();
  buffer = new char[len];

  for (i=0; i<len; i++)
   {
    #ifdef _DEBUG
    if (line[i] > 255) throw L"wchar_t character not ascii";
    #endif
    buffer[i]=(char)line[i];
   } 

  wr = WriteFile(m_hFile, buffer, line.Length(), &bw, NULL);
  if (wr == FALSE)
   {
    m_Error = String::GetLastErrorMsg(GetLastError());
    throw L"out of disk space?";
   }

  delete [] buffer;
 }

 protected:

 bool   m_FirstLine;

};


class AFileTextIn : public AFile
{
 public:

 bool Open(String const &path)
 {
  HANDLE hFile;
  DWORD br;
  BOOL rr;
  byte bchr[2];

  m_IsUnicode = false;

  hFile = CreateFile(path.Chars(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);    
  if (hFile == INVALID_HANDLE_VALUE)
   {
    m_Error = String::GetLastErrorMsg(GetLastError());
    return false;
   }
  else
   {
    rr = ReadFile(hFile, bchr, 2, &br, NULL); 
    if (rr == FALSE)
     {
      m_Error = String::GetLastErrorMsg(GetLastError());
      return false;
     }
    if (br != 2) 
     {
      m_Error = L"File Empty";
      return false;
     }
    if (bchr[0] == 0xFF && bchr[1] == 0xFE)
     {
      m_IsUnicode = true;
     }
    if (m_IsUnicode == false) // rewind
     {
      CloseHandle(hFile);
      hFile = CreateFile(path.Chars(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);    
      if (hFile == INVALID_HANDLE_VALUE)
       {
        m_Error = String::GetLastErrorMsg(GetLastError());
        return false;
       }
     }
    m_hFile = hFile;
    m_Path = path;
    return true;
   }
 }


 std::vector<String> ReadAll()
 {
  std::vector<String> lines;
  String line;
  wchar_t wchr, lastWChr;
  bool eof, gotChr;

  eof = false;
  lastWChr = 0;

  while(!eof)
   {
    if (m_IsUnicode == true)
      gotChr = ReadUnicode(&wchr);
    else
      gotChr = ReadAscii(&wchr);
    if (gotChr == true)
     {
      if (wchr == L'\n' && lastWChr == L'\r')
       {
        lines.push_back(line);
        line.Clear();
       }
      if (wchr>=' ')
        line += wchr;
      lastWChr = wchr; 
     }
    else
     {
      eof = true;
     }
   }
  if (line.Length() > 0)
    lines.push_back(line);

  return lines;
 }

 protected:

 bool ReadAscii(wchar_t *wchr)
 {
  DWORD br;
  BOOL rr;
  bool eof = false;
  char chr;

  rr = ReadFile(m_hFile, &chr, 1, &br, NULL); // 1 byte
  if (rr == FALSE)
   {
    m_Error = String::GetLastErrorMsg(GetLastError());
    throw L"read failed";
   } 
  if (br == 0)
    return false;  // eof
  else
   {
    *wchr = (wchar_t)chr;
    return true;   // byte read
   }
 }

 bool ReadUnicode(wchar_t *chr)
 {
  DWORD br;
  BOOL rr;
  byte bytes[2];
  wchar_t wchr;
  bool eof = false;

  rr = ReadFile(m_hFile, bytes, 2, &br, NULL); // 1 byte
  if (rr == FALSE)
   {
    m_Error = String::GetLastErrorMsg(GetLastError());
    throw L"read failed";
   } 

  if (br == 0)
    return false;  // eof
  else
   {
    if (br != 2)
      throw L"missing final byte of wchar_t";
    wchr = (wchar_t)bytes[0];
    if (bytes[1] != 0)
      wchr |= (wchar_t)(bytes[1] << 8);
   
    *chr = wchr;
    return true;   // byte read
   }
 }


 bool   m_IsUnicode;

};

class LocalOptions
{
 public:

 void SetPath(String const &appDataPath)
 {
  m_Path = appDataPath;
 }

 void SetName(String const &name) 
 { 
  m_Name = name; 
 }

 int LanguageID() 
 { 
  return m_LanguageID; 
 }

 void SaveLanguage(int langID)
 {
  String lang, val;

  m_LanguageID = langID;

  lang = L"Language";
  val = String::Decimal(langID);

  if (Values.count(lang) > 0)
    Values[lang] = val;
  else
    Values.insert(std::pair<String, String>(lang, val));

  Save();
 }


 String GetSetting(String const &key)
 {
  String val;

  val.Clear();

  if (Values.count(key) > 0)
   val = Values[key];
 
  return val;   
 }

 void SaveSetting(String const &key, String const &val)
 {
  if (Values.count(key) > 0)
    Values[key] = val;
  else
    Values.insert(std::pair<String, String>(key, val));
  
  if (Save() == false)
   ::MessageBox(0, m_Error.Chars(), L"Save Local Option File Failed", MB_OK);
 }

 bool Load()
 {
  AFileIn file;
  AFileStr str, val;
  AFileInt c;
  String path, lang;
  int i;

  path = m_Path;
  path += L"\\";
  path += m_Name;

  Values.clear();

  if (file.Open(path) == false)
   {
    m_Error = file.Error();
    return false;
   }

  c = file.ReadInteger();
  if (c.Success == false)
   {
    m_Error = L"Failed to read option count";
    return false;
   }

  for (i=0; i<c.Value; i++)
   {
    str = file.ReadString();
    if (str.Success == false)
     {
      m_Error = L"failed to read option name";
      return false;
     }
    val = file.ReadString();
    if (val.Success == false)
     {
      m_Error = L"failed to read option bytes";
      return false;
     }
    Values.insert(std::pair<String, String>(str.Value, val.Value));
   }
  file.Close();

  m_LanguageID = -1;
  lang = L"Language";
  if (Values.count(lang) > 0)
   {
    if (String::TryIntParse(Values[lang], &i) == true)
     {
      m_LanguageID = i;
     }
   }
  return true;
 }

 bool Save()
 {
  AFileOut file;
  String path;

  path = m_Path;
  path += L"\\";
  path += m_Name;

  if (file.Open(path) == false)
   {
    m_Error = file.Error();
    return false;
   }

  file.Write((int)Values.size());

  for (const auto &opt : Values)
   {
    file.Write(opt.first);
    file.Write(opt.second);
   } 
  file.Close();
  return true;
 }


 String Error() { return m_Error; }

 std::map<String, String> Values;


 protected:

 int    m_LanguageID;
 
 String m_Error;
 String m_Name;
 String m_Path;

};