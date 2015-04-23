#include "UsefullTools.h"
string itos(int x){
	string s  = "";
	string tem="";
	if(x == 0)return "0";
	while(x){
		s += '0' + x % 10;
		x /= 10;
	}
	int l  = s.size();
	for(int i = 0; i < l; ++i){
		tem+=s[l-i-1];
	}
	return tem;
}
Uint16 OFtoUint16(OFString s)
{
	Uint16 result=0;
	Uint16 jw=1;
	for(int i=s.size()-1; i >=0 ;--i)
	{
		result += (s[i]-'0')*jw;
		jw*=10;
	}
	return result;

}
_signal OFtoSignal(OFString s)//OFString ×ª»»µ½ ÐÅºÅ
{
	_signal sum=0;
	_signal underzero=1;
	_signal pow=1;
	for(int i=s.size()-1;i>=0;i--)
	{
		if(s[i]=='.')
		{
			underzero=pow;
		}else if(s[i]=='-')
		{
			sum*=-1;
		}
		else if(s[i]!=' ')
		{
			sum+=pow*(s[i] - '0');
			pow*=10;
		}
	}
	return sum/underzero;
}

