#pragma once
#include <iostream>
#include <string.h>
#include <cassert>
using namespace std;

typedef long long INT64;                  //对long long进行重命名
const INT64 INVALID = 0xcccccccccccccccc; //随机值
const INT64 MAX =  9223372036854775807;   //最大值(0x7FFFFFFFFFFFFFFF)
const INT64 MIN = -9223372036854775808;   //最小值(0x8000000000000000)

class BigData
{
public:
    friend ostream& operator<<(ostream& _cout,const BigData& bd);

    BigData()//无参构造函数
        :_pData("")
        ,_value(INVALID)
    {}
    BigData(INT64 value)//给一个整数串的构造函数(注意要保证MIN<value<MAX，否则会出错)
    {
        if(value==0)//传进来的是0值
        {
            _pData.resize(3);
            _pData[0] = '+';//默认0是正的
            _pData[1] = '0';
            _pData[2] = '\0';
            return;
        }

        _value = value;

        char count = 0;//统计所给整数的位数
        INT64 tmp = value;//给出临时变量tmp做计算
        //统计所给整数有多少位
        while(tmp)
        {
            ++count;
            tmp /= 10;
        }
        _pData.resize(count+2);//给符号和'\0'多开辟俩个字节
        //确认符号
        if (value > 0)
            _pData[0] = '+';
        else
            _pData[0] = '-';

        //从后向前给字符数组赋值
        tmp = value;
        if (tmp < 0)
            tmp = 0 - tmp;//如果tmp是负数，将其改为正数
        for (int i = count; i > 0; --i)
        {
            char c = tmp%10 + '0';//取最低位
            _pData[i] = c;
            tmp /= 10;
        }
    }
    BigData(const char* s)//用字符串构造大数的构造函数
        :_value(0)
    {
        int len = strlen(s);//求所给字符串的长度
        _pData.resize(len+2,0);//为符号位和'\0'多开辟两个空间
        char sign = '+';//默认符号为正
        char* str = (char*)s;//用一个字符串指向s
        size_t size = 0;//记录实际有效的字符串个数

        //跳过前面的'0'和' '(比如"00003526727"  "   6767867")
        while(*str == '0' || *str == ' ')
            ++str;

        //预防"000000" 和 "     "
        if (*str == '\0')
        {
            _value = 0;
            _pData[0] = sign;//默认0为正
            _pData[1] = '0';
            _pData.resize(3);//'+' '0' '\0'
            return;
        }

        //确认符号
        if (str[0] == '-' || str[0] == '+')
        {
            sign = *str++;
        }

        //预防"+000006458"  "-    648"
        while(*str == '0' || *str == ' ')
            ++str;

        //预防"+000000" 和 "-     "
        if (*str == '\0')
        {
            _value = 0;
            _pData[0] = sign;
            _pData[1] = '0';
            _pData.resize(3);
            return;
        }

        //从第1位开始赋值，第0位放符号位
        for (int i = 1; i <= len; ++i)
        {
            //只有遇到数字字符才向字符数组和_value中赋值
            if(*str >= '0' && *str <= '9')
            {
                _value = _value*10 + (*str - '0');
                _pData[i] = *str;
                ++size;
                str++;
            }
            else
            {
                if(i == 1)//处理"abc2654738"
                    _pData[i] = '0';
                break;
            }
        }

        _pData[0] = sign;//将0号位置赋值为符号位
        _pData.resize(size+2);//重置大小    

        //确定_value的符号
        if (sign == '-')
            _value = 0 - _value;
    }

    BigData operator+(BigData& bd)
    {
        BigData* l = this;//左操作数
        BigData* r = &bd;//右操作数

        int lSize = l->_pData.size();
        int rSize = r->_pData.size();
        //将较长的数放于左操作数
        if (lSize < rSize)
        {
            swap(l,r);
            swap(lSize,rSize);
        }
        //如果两个数据长度一样，就将（除符号）大的数放于左操作数
        if (lSize == rSize)
        {
            if (strcmp(l->_pData.c_str()+1,r->_pData.c_str()+1) < 0)
            {
                swap(l,r);
                swap(lSize,rSize);
            }
        }

        //两个数据同号（结果与数据符号相同）
        if (l->_pData[0] == r->_pData[0])
        {
            //两个数据至少有一个溢出了
            if (l->IsOverFlow() || r->IsOverFlow())
                return BigData(l->ADD(r->_pData).c_str());//如果有溢出，调用字符串相加函数

            //两个数据都没溢出，但结果有可能溢出
            else
            {
                //同号正溢出：结果溢出了  5 - 2 < 4
                if (l->_pData[0] == '+' && MAX - l->_value < r->_value)
                {
                    return BigData(l->ADD(r->_pData).c_str());//如果有溢出，调用字符串相加函数
                }
                //同号负溢出：结果溢出了  -5 - (-3) > -3
                else if (l->_pData[0] == '-' && MIN - l->_value > r->_value)
                {
                    return BigData(l->ADD(r->_pData).c_str());//如果有溢出，调用字符串相加函数
                }
                //两个数据都没有溢出，和也没有溢出
                else
                {
                    //直接相加两个数据，然后构造临时对象返回
                    return BigData(l->_value+r->_value);
                }
            }
        } 
        //两个数据异号（结果的符号为数据较大的数据的符号，即左操作数）
        else
        {
            //两个数据都没溢出，结果肯定没溢出
            if (!l->IsOverFlow() && !r->IsOverFlow())
            {
                //直接相加两个数据，然后构造临时对象返回
                return BigData(l->_value+r->_value);
            }
            //两个数据至少有一个溢出了
            else
            {
                return BigData(l->SUB(r->_pData).c_str());//如果有溢出，调用字符串相减函数
            }
        }
    }
    BigData operator-(BigData& bd)
    {
        BigData* l = this;//左操作数
        BigData* r = &bd;//右操作数

        int lSize = l->_pData.size();
        int rSize = r->_pData.size();
        //将较长的数放于左操作数
        if (lSize < rSize)
        {
            swap(l,r);
            swap(lSize,rSize);
        }
        //如果两个数据长度一样，就将（除符号）大的数放于左操作数
        if (lSize == rSize)
        {
            if (strcmp(l->_pData.c_str()+1,r->_pData.c_str()+1) < 0)
            {
                swap(l,r);
                swap(lSize,rSize);
            }
        }

        //两个数据同号
        if (l->_pData[0] == r->_pData[0])
        {
            //两个数据都没溢出，结果肯定也没溢出
            if (!l->IsOverFlow() && !r->IsOverFlow())
            {
                //直接相减两个数据，然后构造临时对象返回
                return BigData(l->_value - r->_value);
            }
            //两个数据至少有一个溢出了,调用字符串相减函数
            else
            {
                return BigData(l->SUB(r->_pData).c_str());
            }
        }
        //两个数据异号
        else
        {
            //两个数据至少有一个溢出了，直接调用字符串相加函数
            if (l->IsOverFlow() || r->IsOverFlow())
            {
                return BigData(l->ADD(r->_pData).c_str());
            }
            //两个数据都没有溢出，结果有可能溢出也有可能不溢出
            else
            {
                //异号正溢出：结果溢出了  5 - 2 < 4
                if (l->_pData[0] == '+' && MAX - l->_value < r->_value)
                {
                    return BigData(l->ADD(r->_pData).c_str());//如果有溢出，调用字符串相加函数
                }
                //异号负溢出：结果溢出了  -5 - (-3) > -3
                else if (l->_pData[0] == '-' && MIN - l->_value > r->_value)
                {
                    return BigData(l->ADD(r->_pData).c_str());//如果有溢出，调用字符串相加函数
                }
                //两个数据都没有溢出，和也没有溢出
                else
                {
                    //直接相减两个数据，然后构造临时对象返回
                    return BigData(l->_value-r->_value);
                }
            }
        }
    }
    BigData operator*(BigData& bd)
    {
        BigData* l = this;//左操作数
        BigData* r = &bd;//右操作数

        int lSize = l->_pData.size();
        int rSize = r->_pData.size();

        //将较长的数放于左操作数
        if (lSize < rSize)
        {
            swap(l,r);
            swap(lSize,rSize);
        }

        //两个数据同号,运算结果符号为正
        if (l->_pData[0] == r->_pData[0])
        {
            //至少有一个数据溢出，调用字符串乘法函数
            if (l->IsOverFlow() || r->IsOverFlow())
            {
                return BigData(l->MUL(r->_pData).c_str());
            }
            //两个数据都没溢出，结果可能溢出也可能不溢出
            else
            {
                //正溢出：结果溢出了  5 / 2 < 4
                if (MAX / l->_value < r->_value)
                {
                    return BigData(l->MUL(r->_pData).c_str());//如果有溢出，调用字符串相乘函数
                }
                //两个数据都没有溢出，积也没有溢出
                else
                {
                    //直接相乘两个数据，然后构造临时对象返回
                    return BigData(l->_value*r->_value);
                }
            }
        }

         //两个数据异号，运算结果为负
        else                       
        {
            //至少有一个数据溢出，调用字符串乘法函数
            if (l->IsOverFlow() || r->IsOverFlow())
            {
                return BigData(l->MUL(r->_pData).c_str());
            }
            //两个数据都没溢出，结果可能溢出也可能不溢出
            else
            {
                //负溢出：结果溢出了  -5 / (-3) > 3
                if (MIN / l->_value > r->_value)
                {
                    return BigData(l->MUL(r->_pData).c_str());//如果有溢出，调用字符串相乘函数
                }
                //两个数据都没有溢出，和也没有溢出
                else
                {
                    //直接相乘两个数据，然后构造临时对象返回
                    return BigData(l->_value*r->_value);
                }
            } 
        }
    }
    BigData operator/(BigData& bd)
    {
        if(strcmp(bd._pData.c_str()+1,"0")==0)//防止除数为0
            assert(false);
        int lSize = _pData.size();
        int rSize = bd._pData.size();

        //左操作数小于右操作数或者被除数为0，结果为0
        if (lSize < rSize  || strcmp(_pData.c_str()+1,"0")==0)
            return BigData((INT64)0);
        //两个数据绝对值相等，结果为1或-1
        else if (lSize == rSize && strcmp(_pData.c_str()+1,bd._pData.c_str()+1)==0)
        {
            if(_pData[0] != bd._pData[0])
                return BigData((INT64)-1);
            else
                return BigData((INT64)1);
        }
        //右操作数为+1或-1，结果为 +左操作数 或 -左操作数
        else if (rSize == 3 && bd._pData[1] == '1')
        {
            if(bd._pData[0] == '+')
            {
                return BigData(_pData.c_str());
            }
            else
            {
                string tmp = _pData;
                tmp[0] = '-';
                return BigData(tmp.c_str());
            }
        }
        else
        {
            //至少有一个数据溢出
            if (IsOverFlow() || bd.IsOverFlow())
            {
                return BigData(DIV(bd._pData).c_str());
            }
            //两个数据都没溢出，结果也没溢出
            else
            {
                return BigData(_value/bd._value);
            }
        }
    }

protected:
    bool IsOverFlow()//判断数据是否溢出
    {
        //先求当前字符串的长度
        int len = _pData.size();

        //与INT64的最大值与最小值的长度作比较，如果大于MAX和MIN的长度，说明溢出
        if (len > 21)//"+9223372036854775807\0"  "-9223372036854775808\0"(21位)
        {
            return true;
        }
        else if (len < 21)//如果大于MAX和MIN的长度，说明溢出
        {
            return false;
        }
        else//如果等于MAX和MIN的长度，则需要继续判断
        {
            if (_pData[0] == '+' && strcmp(_pData.c_str(),"+9223372036854775807")>0)
                return true;
            if (_pData[0] == '-' && strcmp(_pData.c_str(),"-9223372036854775808")>0)
                return true;
            return false;
        }
    }
    string ADD(string& s)//字符串加法
    {
        int lSize = _pData.size();
        int rSize = s.size();

        string ret;//定义返回值
        ret.resize(lSize+1);//99999+99=100098(位数最多为左操作数+1)

        char step = 0;//进位
        char cRet = 0;
        for (int i = 2; i < lSize; ++i)
        {
            //右操作数的位数还没加完，左+右+进位
            if (rSize-i > 0)
            {
                cRet = _pData[lSize-i] - '0' + s[rSize-i] -'0'+ step;
            }
            //右操作数加完后，左+进位
            else
            {
                cRet = _pData[lSize-i] - '0' + step;
            }
            step = cRet/10;
            cRet = cRet%10;
            ret[lSize+1-i] = cRet + '0';
        }
        ret[0] = _pData[0];   //确定符号位
        ret[1] = step+'0';    //保存最后一个进位

        return ret;
    }
    string SUB(string s)//字符串减法
    {
        int lSize = _pData.size();
        int rSize = s.size();

        string ret;//定义返回值
        ret.resize(lSize+1);//99999-(-99)=100098(位数最多为左操作数+1个)

        char step = 0;//借位
        char cRet = 0;
        for (int i = 2; i < lSize; ++i)
        {
            //右操作数的位数还没减完，左-右-借位
            if (rSize-i > 0)
            {
                cRet = (_pData[lSize-i]-'0') - (s[rSize-i]-'0') - step; 
                step = 0;
            }
            //右操作数减完后，左-借位
            else
            {
                cRet = _pData[lSize-i] - '0' - step;
                step = 0;
            }

            if(cRet < 0)//不够减就进行借位
            {
                cRet += 10;//当前0位+10
                step = 1;//向前面借一位
            }

            ret[lSize+1-i] = cRet + '0';
        }
        ret[0] = _pData[0];   //确定符号位
        ret[1] = step+'0';    //保存最后一个借位

        return ret;
    }
    string MUL(string& s)
    {
        int lSize = _pData.size();
        int rSize = s.size();

        string ret;//定义返回值
        //结果最多为lSize+rSize-2位 "+9999\0" * "+99\0" = "+989901\0"
        ret.resize(lSize+rSize-2);

        char step = 0;//进位
        char count = 0;//错位
        char cRet = 0;
        for (int i = 2; i < rSize; ++i)//右操作数比较短，应放于外层循环
        {
            step = 0;
            for (int j = 2; j < lSize; ++j)//左操作数比较长，应放在内层循环
            {
                cRet = (s[rSize-i]-'0') * (_pData[lSize-j]-'0') + step;
                if(count > 0)
                    cRet += ret[lSize+rSize-2-j-count] - '0';
                step = cRet/10;
                cRet %= 10;
                ret[lSize+rSize-2-j-count] = cRet + '0';
            }
            count++;//每次向前错一位
            ret[rSize-1-count] += step + '0';//保存最后一个进位
        }

        //确定结果的符号：同号为正，异号为负
        if (_pData[0] == s[0])
            ret[0] = '+';
        else
            ret[0] = '-';

        return ret;
    }
    string DIV(string& s)
    {
        int lSize = _pData.size();
        int rSize = s.size();

        string ret;//定义返回值
        //结果最多为lSize位 "+9999\0" * "+1\0" = "+9999\0"
        ret.reserve(lSize); 

        string sleft(_pData.c_str(),rSize-1);//被除数
        string sright(s.c_str(),rSize-1);//除数
        int index = rSize-1;
        while(index < lSize)
        {
            //不够除
            while (sleft.size()==sright.size() && strcmp(sleft.c_str(),sright.c_str())<0)
            {
                sleft += _pData[index++];//借位
                ret.push_back('0');
                if (index > lSize)
                    break;
            }
            //够除
            int count = 0;
            while(sleft.size()>sright.size() ||\
                 (sleft.size()==sright.size()&&strcmp(sleft.c_str(),sright.c_str())>0))
            {
                BigData tmp(sleft.c_str());
                sleft = tmp.SUB(BigData(sright.c_str())._pData);
                ++count;
                sleft = BigData(sleft.c_str())._pData;//去掉多余的0
                sleft = string(sleft.c_str(),sleft.size()-1);//去掉后面的'\0'
            }
            ret.push_back(count+'0');
            sleft += _pData[index++];//借位
        }
        //确定结果的符号：同号为正，异号为负
        if (_pData[0] == s[0])
            ret[0] = '+';
        else
            ret[0] = '-';
        return ret;
    }

private:
    string _pData;
    INT64 _value;
};

ostream& operator<<(ostream& _cout,const BigData& bd)
{
    _cout<<bd._pData.c_str();
    return _cout;
}



