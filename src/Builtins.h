inline const char *GetBuiltins() {
  return

      "int operator+ (int x, int y);"
      "int operator- (int x, int y);"
      "int operator* (int x, int y);"
      "int operator/ (int x, int y);"
      "int operator% (int x, int y);"

      "float operator+ (float x, float y);"
      "float operator- (float x, float y);"
      "float operator* (float x, float y);"
      "float operator/ (float x, float y);"

      "float operator+ (int x, float y);"
      "float operator+ (float x, int y);"
      "float operator- (int x, float y);"
      "float operator- (float x, int y);"
      "float operator* (int x, float y);"
      "float operator* (float x, int y);"
      "float operator/ (int x, float y);"
      "float operator/ (float x, int y);"

      "bool operator== (int x, int y);"
      "bool operator!= (int x, int y);"
      "bool operator<  (int x, int y);"
      "bool operator<= (int x, int y);"
      "bool operator>  (int x, int y);"
      "bool operator>= (int x, int y);"
      "bool operator== (float x, float y);"
      "bool operator!= (float x, float y);"
      "bool operator<  (float x, float y);"
      "bool operator<= (float x, float y);"
      "bool operator>  (float x, float y);"
      "bool operator>= (float x, float y);"

      "bool operator== (int x, float y);"
      "bool operator== (float x, int y);"
      "bool operator!= (int x, float y);"
      "bool operator!= (float x, int y);"
      "bool operator<  (int x, float y);"
      "bool operator<  (float x, int y);"
      "bool operator<= (int x, float y);"
      "bool operator<= (float x, int y);"
      "bool operator>  (int x, float y);"
      "bool operator>  (float x, int y);"
      "bool operator>= (int x, float y);"
      "bool operator>= (float x, int y);"

      "bool operator! (bool x);"
      "int operator- (int x);"
      "float operator- (float x);"

      "bool operator&& (bool x, bool y);"
      "bool operator|| (bool x, bool y);"

      "bool operator bool (int x);"
      "int operator int (bool x);"
      "bool operator bool (float x);"
      "float operator float (bool x);"
      "int operator int (float x);"
      "float operator float (int x);"

      "int print(int x);"
      "int print(float x);"
      "int print(bool x);";
}
