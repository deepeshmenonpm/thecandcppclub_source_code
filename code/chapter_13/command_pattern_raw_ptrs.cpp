#include <iostream>
#include <string>
#include <memory>
#include <vector>

using namespace std;

class Command
{
public:
    virtual ~Command(){}
    virtual void execute() = 0;
};

class Menu_Item
{
private:
    Command *command_object_;
    string name_;
public:
    Menu_Item(const char* const name) : name_{name}
    {
        command_object_ = nullptr;
    }
    
    void add_command_object(Command *command_object)
    {
        command_object_ = command_object;
    }
    
    int click()
    {
        int status = -1;
        if(command_object_ != nullptr)
        {
            command_object_->execute();
            status = 0;
        }
        
        return status;
    }
    
    string& get_name()
    {
        return name_;
    }
};

class Menu
{
private:
    string name_;
    vector<Menu_Item*> menu_items;
public:
    Menu(const char* const name): 
        name_{name}
    {
        
    }
    
    void add_menu_item(Menu_Item *menu_item)
    {
        menu_items.push_back(menu_item);
    }
    
    int hover()
    {
        int choice;
        int count = 0;
        
        for (auto item : menu_items)
        {
            cout << item->get_name() << " : " << count++ << endl;
        }
        
        cout << "Enter your Menu Item choice : ";
        cin >> choice;
        
        if(   (choice >= 0) 
            &&(choice <= menu_items.size()) )
        {
            return menu_items[choice]->click();
        }
        
    }
    
    string& get_name()
    {
        return name_;
    }
    
};

class Document
{
private:
    string name_;
public:
    typedef void (Document::*doc_function)();
    Document(const char* const name): name_{name}{}
    
    ~Document()
    {
        close();
    }
    
    void open()
    {
        cout << "Document : " << name_ << " is open" << endl;
    }
    
    void close()
    {
        cout << "Document : " << name_ << " is closed" << endl;
    }
    
    //void copy(int start, int end, string &copied_text)
    void copy()
    {
        cout << "copying..." << endl;
        // copy from start to end to copied_text
        cout << "copy complete" << endl;
    }
    
    //void paste(int position, string& insert_text)
    void paste()
    {
        cout << "pasting..." << endl;
        cout << "pasting complete" << endl;
    }
    
    //void cut(int start, int end, string &extracted_text)
    void cut()
    {
        cout << "cutting..." << endl;
        // copy from start to end to copied_text
        cout << "cut complete" << endl;
    }
    
};

class Open_Document_Command : public Command
{
private:
    Document* document_;
    Document::doc_function function;

public:
    Open_Document_Command(Document* document, Document::doc_function call_back):
        document_{document}, function{call_back}
    {
        
    }
    void execute() override
    {
        (document_->*function)();
    }
};

class Close_Document_Command : public Command
{
private:
    Document* document_;
    Document::doc_function function;

public:
    Close_Document_Command(Document* document, Document::doc_function call_back):
        document_{document}, function{call_back}
    {
        
    }
    void execute() override
    {
        (document_->*function)();
    }
};


class Copy_Document_Command : public Command
{
private:
    Document* document_;
    Document::doc_function function;

public:
    Copy_Document_Command(Document* document, Document::doc_function call_back):
        document_{document}, function{call_back}
    {
        
    }
    void execute() override
    {
        (document_->*function)();
    }
};

class Paste_Document_Command : public Command
{
private:
    Document* document_;
    Document::doc_function function;

public:
    Paste_Document_Command(Document* document, Document::doc_function call_back):
        document_{document}, function{call_back}
    {
        
    }
    void execute() override
    {
        (document_->*function)();
    }
};

class Cut_Document_Command : public Command
{
private:
    Document* document_;
    Document::doc_function function;

public:
    Cut_Document_Command(Document* document, Document::doc_function call_back):
        document_{document}, function{call_back}
    {
        
    }
    
    void execute() override
    {
        (document_->*function)();
    }
};


class Application
{
private:
    vector<Menu*> menus;
    Document *document;
    Menu *file_menu;
    Menu *edit_menu;
    Menu_Item *open_item;
    Menu_Item *close_item;
    Menu_Item *paste_item;
    Menu_Item *copy_item;
    Menu_Item *cut_item;
    Open_Document_Command *open_cmd_object;
    Close_Document_Command *close_cmd_object;
    Paste_Document_Command *paste_cmd_object;
    Copy_Document_Command *copy_cmd_object;
    Cut_Document_Command *cut_cmd_object;
    
public:
    Application()
    {
        string doc_name;
        
        cout << "Document editor app" << endl;
        // 1. open a document first for editing 
        cout << "Enter the file name to edit" << endl;
        cin >> doc_name;
        
        document = new Document(doc_name.c_str());
        
        // 2. add Menus "File" and "Edit"
        file_menu = new Menu("File");
        open_item = new Menu_Item("Open");
        open_cmd_object = new Open_Document_Command(document, &Document::open);
        open_item->add_command_object(open_cmd_object);
        
        file_menu->add_menu_item(open_item);
        
        close_item = new Menu_Item("Close");
        close_cmd_object = new Close_Document_Command(document, &Document::close);
        close_item->add_command_object(close_cmd_object);
        
        file_menu->add_menu_item(close_item);
        
        edit_menu = new Menu("Edit");
        copy_item = new Menu_Item("Copy");
        copy_cmd_object = new Copy_Document_Command(document, &Document::copy);
        copy_item->add_command_object(copy_cmd_object);
        
        edit_menu->add_menu_item(copy_item);
        
        paste_item = new Menu_Item("Paste");
        paste_cmd_object = new Paste_Document_Command(document, &Document::paste);
        paste_item->add_command_object(paste_cmd_object);
        
        edit_menu->add_menu_item(paste_item);
        
        cut_item = new Menu_Item("Cut");
        cut_cmd_object = new Cut_Document_Command(document, &Document::cut);
        cut_item->add_command_object(cut_cmd_object);
        
        edit_menu->add_menu_item(cut_item);
        
        menus.push_back(file_menu);
        menus.push_back(edit_menu);
    }
    
    int select_menu()
    {
        int choice = 0;
        int end_app = 0;
        cout << "Enter your Menu choice" << endl;
        for(auto menu : menus)
        {
            cout << menu->get_name() << " : " << choice++ << endl;
        }
        end_app = choice;
        cout << "Exit : " << end_app << endl;
        
        cin >> choice;
        
        if(  (choice >= 0)
           &&(choice < menus.size()))
        {
            menus[choice]->hover();
        }
        else if(choice == end_app)
        {
            end_application();
            return 1;
        }
        
        return 0;
    }
    
    void end_application()
    {
        // Delete the items in the reverse order  
        // How to close the document ?
        // 2. delete the command_objects created 
        delete open_cmd_object;
        delete close_cmd_object;
        delete paste_cmd_object;
        delete copy_cmd_object;
        delete cut_cmd_object;
        // 3. Delete menu items created 
        delete open_item;
        delete close_item;
        delete paste_item;
        delete copy_item;
        delete cut_item;
        // 4. delete menus 
        delete file_menu;
        delete edit_menu;
        // 5. finally delete document
        delete document;
        
        cout << "Application exiting..." << endl;
    }
    
};

int main()
{
    Application new_app;
    int end_app = 0;
    
    while(end_app == 0)
    {
        end_app = new_app.select_menu();
    }
    
    cout << "Main Ends" << endl; 
}

