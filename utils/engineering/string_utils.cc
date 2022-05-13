// Split the input string by delimiter, return results in a vector.
// Delimiters are not returned.
void split(const string& str, char delim, vector<string>& result) {
  size_t cur = 0;
  size_t pos = str.find(delim);
  while (pos != string::npos) {
    if (pos != cur) {
      result.push_back(str.substr(cur, pos - cur));
    }
    pos++;
    cur = pos;
    pos = str.find(delim, pos);
  }
  if (cur != str.length()) {
    result.push_back(str.substr(cur));
  }
}
