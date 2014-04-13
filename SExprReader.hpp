#ifndef SEXPRREADER_H
#define SEXPRREADER_H

#include <string>
#include <list> 
#include <iostream>


class SExprObject
{
public:
    enum Type
    {
        TYPE_UNSPECIFIED,
        TYPE_LIST,
        TYPE_SYMBOL,
        TYPE_NUMBER,
        TYPE_STRING
    };
    
    typedef std::list<SExprObject*> SExprObjectList;
    
    
public:
    SExprObject ();
    ~SExprObject ();
    SExprObject (const SExprObject& other);
    
    void clear_list();

    Type get_type () const;
    
    const std::string& get_string () const;
    const std::string& get_symbol_name () const;
    int get_number () const;
    const SExprObjectList& get_list () const;
    
    void set_string (const std::string& string);
    void set_symbol_name (const std::string& name);
    void set_number (int number);
    void set_list (const SExprObjectList& list);

    void print (std::ostream& stream) const;
    

    friend std::ostream& operator<< (std::ostream& out, const SExprObject& obj);
    
    
protected:
    Type _type;
    std::string _string;
    std::string _name;
    int _number;
    SExprObjectList _list;
};



class SExprReader
{
public:
    SExprReader (std::istream& input);
    ~SExprReader ();
    
    void read (SExprObject** obj);
    
protected:
    void read_int (SExprObject** obj);
    void read_string (SExprObject** obj);
    void read_symbol (SExprObject** obj);
    void read_number (SExprObject** obj);
    void read_list (SExprObject** obj);
    
    int peek_char ();
    int read_char ();
    void eat_whitespace ();
    void eat_comment ();
    void error (const char* message);
    bool is_whitespace (char c) const;
    bool is_numeric (char c) const;
    bool is_symbol_char (char c) const;


    void log (const char* msg) const;

protected:
    std::istream& _input;
};

#endif // SEXPRREADER_H
