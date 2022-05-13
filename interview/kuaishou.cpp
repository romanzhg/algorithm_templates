#include <iostream>
#include <cstring>

using namespace std;

void f(const char* s) {
    int len = strlen(s);
    for (int i = 0; i < len; i++) {
        if (i == len - 1) {
            cout << s[i];
        }
        if (s[i] == '/' && s[i + 1] == '/') {
            i += 2;
            while (i < len && s[i] != '\n') {
                i++;
            }
        } else if (s[i] == '/' && s[i + 1] == '*') {
            i += 2;
            while ((i + 1) < len && (s[i] != '*' || s[i + 1] != '/')) {
                i++;
            }
            i++;
        } else {
            cout << s[i];
        }
    }
}

int main() {
    //int a;
    //cin >> a;
    cout << "Hello World!" << endl;
    string tmp = "\
        int main() {\n\
            //int a;\n\
            //cin >> a;\n\
            cout << \"Hello World!\" << endl;\n\
            /** some comments ***/\n\
            /** some co//mments ***/\n\
            // /**/\n\
            cout << endl;\
        }\n\
      ";
    f(tmp.c_str());
    return 0;
}
