#include "staticstics.hpp"

using std::endl;

ostream& operator<<(ostream& os, const Statistics& st){
    os<<"Statistics:"<<endl;
    os<<"Files: "<<st.files<<endl;
    os<<"Lines: "<<st.non_empty_lines + st.empty_lines<<", including:"<<endl;
    os<<"    non-empty: "<<st.non_empty_lines<<endl;
    os<<"    empty ones: "<<st.empty_lines<<endl;
    os<<"Words: "<<st.words<<endl;
    os<<"Letters: "<<st.letters<<endl;
    return os;
}

Statistics Statistics::operator+(const Statistics& st){
    return Statistics{
        .files = this->files + st.files,
        .non_empty_lines = this->non_empty_lines + st.non_empty_lines,
        .empty_lines = this->empty_lines + st.empty_lines,
        .words = this->words + st.words,
        .letters = this->letters + st.letters
    };
}

Statistics& Statistics::operator+=(const Statistics& st){
    this->files += st.files;
    this->non_empty_lines += st.non_empty_lines;
    this->empty_lines += st.empty_lines;
    this->words += st.words;
    this->letters += st.letters;
    return *this;
}

bool Statistics::operator==(const Statistics& st){
    return this->files == st.files &&
            this->non_empty_lines == st.non_empty_lines &&
            this->empty_lines == st.empty_lines &&
            this->words == st.words &&
            this->letters == st.letters;
}