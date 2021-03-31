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

int find_length(int len)
{
    int r=0;
    while(1)
    {
        if(pow(2,r) >= len + r + 1)
            break;
        r++;
    }

    return (len + r);
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

string process_string(string str, int redun_len)
{
    string res=str;
    int i=0;
    while(i<redun_len)
    {
        int pos_bit = 1<<i;

        vector<int> positions = find_positions(pos_bit,str.length());
        // print_vector(positions);

        int parity_bit = get_parity_bit(positions,res);
        // cout<<parity_bit<<" -> ";

        res[pos_bit-1]=parity_bit+'0';
        // cout<<res<<"\n";
        i++;
    }
    return res;
}
int main()
{
    string inp;
    cout<<"Enter the binary string: ";
    cin>>inp;

    int code_len=find_length(inp.length());

    // cout<<inp<<" "<<inp.length()<<" "<<code_len<<"\n";

    string unproc_string = generate_unprocessed_code(inp,code_len);
    
    // cout<<unproc_string<<"\n";

    string hamming_code = process_string(unproc_string,code_len - inp.length());

    cout<<"Generated hamming code is:\n"<<hamming_code<<"\n";
    return 0;
}