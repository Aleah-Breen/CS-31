#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <cctype>
#include <cstring>
using namespace std;

int render(int lineLength, istream& inf, ostream& outf);

bool getToken(char word[], istream& inf, bool& procPara, ostream& outf, bool& newPara)
{
    char c;
    bool check = false;
    while (inf.get(c))
    {
        // Start a new paragraph if called and not the end of the file
        if (procPara){
            outf << endl << endl;
            procPara = false;
            newPara = true;
        }
        
        // Add character to the token cstring
        if (!isspace(c))
        {
            char single[2] = {c, '\0'};
            strcat(word, single);
            check = true;
        }
        
        // Return if end of the word
        if (check && (isspace(c) || c == '-'))
        {
            return true;
        }
    }
    
    // Send the last word of the file
    if (check)
    {
        return true;
    }
    
    // End of file
    return false;
}


int render(int lineLength, istream& inf, ostream& outf)
{
    if (lineLength < 1)
    {
        return 2;
    }
    
    int currentLine = 0;
    bool split = false;
    char prev[180] = "";
    bool newPara = true;
    bool procPara = false;


    for (;;)
    {
        bool dash = false;
        int spaces = 0;
        char token[180] = "";
        
        // Get the token and end loop if file end
        if (!getToken(token, inf, procPara , outf, newPara))
        {
            outf << endl;
            break;
        }
        
        // Start a new paragraph
        if (strcmp(token, "@P@") == 0)
        {
            if (!newPara)
            {
                procPara = true;
                currentLine = 0;
                strcpy(prev, token);
            }

        }
        
        // Process word
        else
        {
            // If a word is longer than line length
            if (strlen(token) > lineLength)
            {
                split = true;
                if (currentLine != 0)
                {
                    outf << endl;
                }
                currentLine = 0;
                
                // Print character by character until the line is full
                for (int i = 0; i < strlen(token); i++)
                {
                    if (i % lineLength == 0 && i != 0)
                    {
                        outf << endl;
                        currentLine = 0;
                    }
                    outf << token[i];
                    currentLine += 1;
                }
            }
            
            else if (strlen(token) <= lineLength)
            {
                
                // Analyze the previous word for punctuation
                for (int i = 0; prev[i] != '\0'; i++)
                {
                    if (prev[i + 1] == '\0' && (prev[i] == '.' || prev[i] == '?' || prev[i] == '!' || prev[i] == ':') && currentLine != 0 && currentLine + (strlen(token) + 2) <= lineLength)
                    {
                        outf << " ";
                        spaces += 1;
                    }
                    
                    else if (prev[i + 1] == '\0' && (prev[i] == '-'))
                    {
                        dash = true;
                    }
                }
                
                // Add a space before a new word
                if (currentLine != 0 && !dash && currentLine + (strlen(token) + 1) <= lineLength)
                {
                    outf << " ";
                    spaces += 1;
                }
                
                // Prints the word if it fits in the line
                if ((spaces == 2 && currentLine + spaces + strlen(token) <= lineLength) || (spaces == 1 && currentLine + spaces + strlen(token) <= lineLength) || (dash && currentLine + strlen(token) <= lineLength) || newPara)
                {
                    outf << token;
                }
                
                // Moves the word to the next line if too long
                else
                {
                    outf << endl << token;
                    currentLine = 0;
                }
                
                currentLine += strlen(token) + spaces;
            }
            strcpy(prev, token);
            newPara = false;

        }
        
    }
    
    
    if (split)
    {
        return 1;
    }
    
    return 0;
}

int top()
{
    const int MAX_FILENAME_LENGTH = 100;
    for (;;)
    {
        cout << "Enter input file name (or q to quit): ";
        char filename[MAX_FILENAME_LENGTH];
        cin.getline(filename, MAX_FILENAME_LENGTH);
        if (strcmp(filename, "q") == 0)
            break;
        ifstream infile(filename);
        if (!infile)
        {
            cerr << "Cannot open " << filename << "!" << endl;
            continue;
        }
        cout << "Enter maximum line length: ";
        int len;
        cin >> len;
        cin.ignore(10000, '\n');
        int returnCode = render(len, infile, cout);
        cout << "Return code is " << returnCode << endl;
    }
}

#include <iostream>
#include <sstream>
#include <streambuf>
#include <string>
#include <cassert>
using namespace std;

class LimitOutputStreambuf : public streambuf
{
    public:
        LimitOutputStreambuf(streambuf* sb, size_t lim)
         : real_streambuf(sb), limit(lim)
        {
                static char dummy;
                setp(&dummy, &dummy);
        }
    private:
        streambuf* real_streambuf;
        size_t limit;

        int overflow(int c)
        {
                if (limit == 0)
                        return traits_type::eof();
                limit--;
                return real_streambuf->sputc(c);
        }
};

class StreambufSwitcher
{
    public:
    StreambufSwitcher(ios& dest, streambuf* sb,
                    ios::iostate exceptions = ios::goodbit)
     : dest_stream(dest), oldsb(dest.rdbuf(sb)), oldex(dest.exceptions())
    { dest_stream.exceptions(exceptions); }
    StreambufSwitcher(ios& dest, ios& src)
     : StreambufSwitcher(dest, src.rdbuf(), src.exceptions())
    {}
    ~StreambufSwitcher()
    { dest_stream.rdbuf(oldsb); dest_stream.exceptions(oldex); }
    private:
    ios& dest_stream;
    streambuf* oldsb;
    ios::iostate oldex;
};

string repeat(size_t n, string s)
{
    string result;
    result.reserve(n * s.size());
    for ( ; n > 0; n--)
        result += s;
    return result;
}

int render(int lineLength, istream& inf, ostream& outf);

void testone(int n)
{
    istringstream iss;
    StreambufSwitcher ssi(cin, iss.rdbuf());

    ostringstream oss;
    LimitOutputStreambuf lob(oss.rdbuf(), 1000*1000+10);
    StreambufSwitcher sso(cout, &lob);

    LimitOutputStreambuf lob2(cerr.rdbuf(), 0);
    StreambufSwitcher sse(cerr, &lob2);

    switch (n)
    {
                                 default: {
        cout << "Bad argument" << endl;
                        } break; case  1: {
        iss.str("hello there\n\nthis is a test\n");
        assert(render(0, iss, oss) == 2);
                        } break; case  2: {
        iss.str("hello there\n\nthis is a test\n");
        render(0, iss, oss);
        assert(oss.str().empty());
                        } break; case  3: {
        iss.str("hello there\n\nthis is a test\n");
        ostringstream oss2;
        StreambufSwitcher sso2(cout, oss2.rdbuf());
        render(11, iss, oss);
        assert(oss2.str().empty());
                        } break; case  4: {
        iss.str("hello there\n\nthis is a test\n");
        istringstream iss2("Z Y X W V\n");
        StreambufSwitcher ssi2(cin, iss2.rdbuf());
                render(11, iss, oss);
        string s = oss.str();
        assert(s.find_first_of("ZYXWV") == string::npos  &&
        s.find("hello") != string::npos);
                        } break; case  5: {
        iss.str("");
        assert(render(10, iss, oss) == 0  &&  oss.str().empty());
                        } break; case  6: {
        iss.str("     \n\n    \n\n");
        assert(render(10, iss, oss) == 0  &&  oss.str().empty());
                        } break; case  7: {
        iss.str("abcdefghi abcdefghijk\n");
        assert(render(10, iss, oss) == 1);
                        } break; case  8: {
        iss.str("abcdefghi abcdefghijk abc abc abc abc abc abc abc\n");
        assert(render(10, iss, oss) == 1);
                        } break; case  9: {
        iss.str("abcdefghi abcdefghij\n");
        assert(render(10, iss, oss) == 0);
                        } break; case 10: {
        iss.str("abcdefghi abcdefgh-ijk\n");
        assert(render(10, iss, oss) == 0);
                        } break; case 11: {
        iss.str("hello there\n");
        render(30, iss, oss);
        assert(oss.str().find("hello there") != string::npos);
                        } break; case 12: {
        iss.str("hello       there\n");
        render(30, iss, oss);
        assert(oss.str().find("hello there") != string::npos);
                        } break; case 13: {
        iss.str("hello. there? bye\n");
        render(30, iss, oss);
        assert(oss.str().find("hello.  there?  bye") != string::npos);
                        } break; case 14: {
        iss.str("hello! there: bye\n");
        render(30, iss, oss);
        assert(oss.str().find("hello!  there:  bye") != string::npos);
                        } break; case 15: {
        iss.str("hello? there\n");
        render(30, iss, oss);
        assert(oss.str().find("hello?  there") != string::npos);
                        } break; case 16: {
        iss.str("hello. abcdefghi\n");
        render(10, iss, oss);
        assert(oss.str().find("hello.\na") != string::npos);
                        } break; case 17: {
        iss.str("hello.there\n");
        render(30, iss, oss);
        assert(oss.str().find("hello.there") != string::npos);
                        } break; case 18: {
        iss.str("hello'3*%there\n");
        render(30, iss, oss);
        assert(oss.str().find("hello'3*%there") != string::npos);
                        } break; case 19: {
        iss.str("hello-there\n");
        render(30, iss, oss);
        assert(oss.str().find("hello-there") != string::npos);
                        } break; case 20: {
        iss.str("hello\nthere\n");
        render(30, iss, oss);
        assert(oss.str().find("hello there") != string::npos);
                        } break; case 21: {
        iss.str("abcdefg hijkl\n");
        render(10, iss, oss);
        assert(oss.str().find("abcdefg\n") != string::npos);
                        } break; case 22: {
        iss.str("abcdefg hijkl\n");
        render(10, iss, oss);
        assert(oss.str().find("\nhijkl") != string::npos);
                        } break; case 23: {
        iss.str("hellohello abcdef ghi jklmnop\n");
        render(10, iss, oss);
        assert(oss.str().find("\nabcdef ghi\n") != string::npos);
                        } break; case 24: {
        iss.str("hellohello abcdef ghi jklmnop qrst\n");
        render(10, iss, oss);
        assert(oss.str().find("\nqrst") != string::npos);
                        } break; case 25: {
        iss.str("abcdefg x-ray\n");
        render(10, iss, oss);
        assert(oss.str().find("x-\nray") != string::npos);
                        } break; case 26: {
        iss.str("abcdefg abc xyz-pq-rs\n");
        render(10, iss, oss);
        assert(oss.str().find("xyz-\npq-rs") != string::npos);
                        } break; case 27: {
        iss.str("abcdefg abc x-yz-pqr\n");
        render(10, iss, oss);
        assert(oss.str().find("x-yz-\npqr") != string::npos);
                        } break; case 28: {
        iss.str("abcdefg abc xyz----yz\n");
        render(10, iss, oss);
        assert(oss.str().find("xyz---\n-yz") != string::npos);
                        } break; case 29: {
        iss.str("abcdefg -xyz\n");
        render(10, iss, oss);
        assert(oss.str().find(" -\nxyz") != string::npos);
                        } break; case 30: {
        iss.str("abcdefg abc abcdefghijklmnopqrstuvw\n");
        render(10, iss, oss);
        assert(oss.str().find("\nabcdefghij\nklmnopqrst\nuvw") != string::npos);
                        } break; case 31: {
        iss.str("abc\n");
        render(10, iss, oss);
        assert(oss.str() == "abc\n");
                        } break; case 32: {
        iss.str("abc @P@ def\n");
        render(10, iss, oss);
        assert(oss.str().find("abc\n\ndef") != string::npos);
                        } break; case 33: {
        iss.str("abc @P@  @P@ def\n");
        render(10, iss, oss);
        assert(oss.str().find("abc\n\ndef") != string::npos);
                        } break; case 34: {
        iss.str("abc\n@P@\ndef\n");
        render(10, iss, oss);
        assert(oss.str().find("abc\n\ndef") != string::npos);
                        } break; case 35: {
        iss.str("@P@ abc @P@ def\n");
        render(10, iss, oss);
        string result("abc\n\ndef\n");
        assert(oss.str() == result  ||
                result.compare(0, 8, oss.str()) == 0);
                        } break; case 36: {
        iss.str("abc @P@ def\n@P@\n");
        render(10, iss, oss);
        assert(oss.str() == "abc\n\ndef\n");
                        } break; case 37: {
        iss.str("@P@ @P@ @P@ abc @P@ def @P@ @P@ @P@\n");
        render(30, iss, oss);
        assert(oss.str() == "abc\n\ndef\n");
                        } break; case 38: {
        string s25 = "abcdefghijklmnopqrstuvwxy";
        string s178(repeat(7, s25) + "abc");
        iss.str(s178 + "\n");
        assert(render(179, iss, oss) == 0);
        string result(s178 + "\n");
        assert(oss.str() == result  ||
                result.compare(0, 178, oss.str()) == 0);
                        } break; case 39: {
        string s24 = "abcdefghijklmnopqrstuvwx";
        string s123(repeat(5, s24) + "abc");
        iss.str(s123 + "\n" + s123 + "\n");
        assert(render(249, iss, oss) == 0);
        string result(s123 + " " + s123 + "\n");
        assert(oss.str() == result  ||
                result.compare(0, 247, oss.str()) == 0);
                        } break; case 40: {
        string s10 = "a\na\na\na\na\na\na\na\na\na\n";
        string s100(repeat(10, s10));
        string s20000(repeat(200, s100)); // For some people, 200 was 20000
        iss.str(s20000);
        render(2, iss, oss);
        assert(oss.str() == s20000  ||
                s20000.compare(0, 19999, oss.str()) == 0);
                        } break; case 41: {
        string s25 = "abcdefghijklmnopqrstuvwxy";
        string s178(repeat(7, s25) + "abc");
        iss.str(repeat(11, s178 + "\n"));
        int n = render(2000, iss, oss);
        string result(repeat(11-1, s178 + " ") + s178 + "\n");
        assert((n == 2  &&  oss.str().empty())  ||  (n == 0  &&
                (oss.str() == result  ||
                 result.compare(0, 11*(178+1)-1, oss.str()) == 0)));
                        } break; case 42: {
        string s25 = "abcdefghijklmnopqrstuvwxy";
        string s178(repeat(7, s25) + "abc");
        iss.str(repeat(550, s178 + "\n"));
        assert(render(99000, iss, oss) == 0);
        string result(repeat(550-1, s178 + " ") + s178 + "\n");
        assert(oss.str() == result  ||
                 result.compare(0, 550*(178+1)-1, oss.str()) == 0);
                        } break;
    }
}

int main()
{
    cout << "Enter test number: ";
    int n;
    cin >> n;
    testone(n);
    cout << "Passed" << endl;
}
