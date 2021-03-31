#include<bits/stdc++.h>
using namespace std;

void print_vector(vector<int> v)
{
    for(int i=0;i<v.size();i++)
    {
        cout<<v[i]<<" ";
    }
    cout<<"\n";
}

int find_redun_len(int len)
{
    int r=0;
    while(1)
    {
        if(pow(2,r) >= len)
            break;
        r++;
    }

    return r;
}

string generate_unprocessed_code(string inp, int len)
{
    int sz=0, index=0;
    string res(len,'*');
    while(sz<len)
    {
        if(ceil(log2(sz+1)) != log2(sz+1))
        {
            res[sz] = inp[index++];
        }
        sz++;
    }

    return res;
}

vector<int> find_positions(int pos_bit, int len)
{
    vector<int> pos;
    for(int i=0;i<len;i++)
    {
        if((i+1)&pos_bit)
            pos.push_back(i);
    }

    return pos;
}

int get_parity_bit(vector<int> positions, string inp_str)
{
    int cnt = 0;
    for(int i=0;i<positions.size();i++)
    {
        if(inp_str[positions[i]]=='1')
            cnt++;
    }
    if(cnt%2==0)
        return 0;
    return 1;
}

int correction(string str, int redun_len)
{
    int i=0;
    int change_position=0;
    while(i<redun_len)
    {
        int pos_bit = 1<<i;

        vector<int> positions = find_positions(pos_bit,str.length());
        // print_vector(positions);

        int parity_bit = get_parity_bit(positions,str);
        // cout<<parity_bit<<" -> ";

        if(parity_bit == 1)
            change_position+=pow(2,i);
        // cout<<parity_bit<<" -> "<<pow(2,i)<<" -> "<<change_position<<"\n";
        i++;
    }
    return change_position;
}
int main()
{
    string inp;
    cout<<"Enter the binary string: ";
    cin>>inp;

    int redun_len=find_redun_len(inp.length());

    // cout<<inp<<" "<<inp.length()<<" "<<redun_len<<"\n";

    int position = correction(inp,redun_len);

    if(position==0)
    {
        cout<<"Gongratulations!!Given hamming code has no errors!!\n";
    }
    else
    {
        cout<<"Position where bit needs to be flipped: "<<position<<"\n";
        cout<<"Final hamming code:\n";
        inp[position-1] = (inp[position-1]=='1')?'0':'1';
        cout<<inp<<"\n";
    }
    
    return 0;
}

/*
001101101101
001101100101

011000100011101101011001111
011000100011100101011001111

0001010011110101100110101010111110
0001010011110101100110101000111110
*/