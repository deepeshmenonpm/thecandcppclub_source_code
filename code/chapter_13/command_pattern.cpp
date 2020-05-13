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
    unique_ptr<Command> command_object_;
    string name_;
public:
    Menu_Item(const char* const name) : 
        name_{name},
        command_object_{nullptr}
    {
    }
    
    ~Menu_Item()
    {
        cout << "Menu_Item : " << name_ << " destroyed" << endl;
        command_object_.reset();        
    }
    
    void add_command_object(Command *command_object)
    {
        command_object_.reset(command_object);
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
    vector<unique_ptr<Menu_Item>> menu_items;
public:
    Menu(const char* const name): 
        name_{name}
    {
        
    }
    
    ~Menu()
    {
        cout << "Menu " << name_ << " destroyed" << endl;
        for (auto &item : menu_items)
        {
            item.reset();
        }
    }
    
    void add_menu_item(unique_ptr<Menu_Item> menu_item)
    {
        menu_items.push_back(std::move(menu_item));
    }
    
    int hover()
    {
        int choice;
        int count = 0;
        
        for (auto &item : menu_items)
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
    
    // overloaded version 
    // used for mimicking a menu item click by the application  
    int hover(int choice)
    {
        return menu_items[choice]->click();
    }
    
    string& get_name()
    {
        return name_;
    }
    
};


class Application;

class Document
{
private:
    string name_;
    Application *app_;
    void (Application::*notify)();
public:
    typedef void (Document::*doc_function)();
    Document( const char* const name, Application *app, void (Application::*app_notify)() ): 
    name_{name}, 
    app_{app},
    notify{app_notify}
    { 
        /* do nothing */ 
    }
    
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
        // notify the application 
        (app_->*notify)();
        
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
    shared_ptr<Document> document_;
    Document::doc_function function;

public:
    Open_Document_Command(shared_ptr<Document> document, Document::doc_function call_back):
        document_{document}, function{call_back}
    {
        
    }
    void execute() override
    {
        (document_.get()->*function)();
    }
};

class Close_Document_Command : public Command
{
private:
    shared_ptr<Document> document_;
    Document::doc_function function;

public:
    Close_Document_Command(shared_ptr<Document> document, Document::doc_function call_back):
        document_{document}, function{call_back}
    {
        
    }
    void execute() override
    {
        (document_.get()->*function)();
    }
};


class Copy_Document_Command : public Command
{
private:
    shared_ptr<Document> document_;
    Document::doc_function function;

public:
    Copy_Document_Command(shared_ptr<Document> document, Document::doc_function call_back):
        document_{document}, function{call_back}
    {
        
    }
    void execute() override
    {
        (document_.get()->*function)();
    }
};

class Paste_Document_Command : public Command
{
private:
    shared_ptr<Document> document_;
    Document::doc_function function;

public:
    Paste_Document_Command(shared_ptr<Document> document, Document::doc_function call_back):
        document_{document}, function{call_back}
    {
        
    }
    void execute() override
    {
        (document_.get()->*function)();
    }
};

class Cut_Document_Command : public Command
{
private:
    shared_ptr<Document> document_;
    Document::doc_function function;

public:
    Cut_Document_Command(shared_ptr<Document> document, Document::doc_function call_back):
        document_{document}, function{call_back}
    {
        
    }
    
    void execute() override
    {
        (document_.get()->*function)();
    }
};


class Application
{
private:
    vector<unique_ptr<Menu>> menus;
    enum class signal{app_launched = 0, app_exit, user_interrupt, other, no_trigger};
    signal trigger;
public:
    Application()
    {
        string doc_name;
        trigger = signal::app_launched;
        
        cout << "Document editor app" << endl;
        // 1. open a document first for editing 
        cout << "Enter the file name to edit" << endl;
        cin >> doc_name;
        
        shared_ptr<Document> document = make_shared<Document>(doc_name.c_str(), this, &Application::notify_doc_close);
        
        // 2. add Menus "File" and "Edit"
        unique_ptr<Menu> file_menu = make_unique<Menu>("File");
        unique_ptr<Menu_Item> open_item = make_unique<Menu_Item>("Open");
        Open_Document_Command *open_cmd_object = new Open_Document_Command(document, &Document::open);
        open_item->add_command_object(open_cmd_object);
        
        file_menu->add_menu_item(std::move(open_item));
        
        unique_ptr<Menu_Item> close_item = make_unique<Menu_Item> ("Close");
        Close_Document_Command *close_cmd_object = new Close_Document_Command(document, &Document::close);
        close_item->add_command_object(close_cmd_object);
        
        file_menu->add_menu_item(std::move(close_item));
        
        unique_ptr<Menu> edit_menu = make_unique<Menu>("Edit");
        unique_ptr<Menu_Item> copy_item = make_unique<Menu_Item>("Copy");
        Copy_Document_Command *copy_cmd_object = new Copy_Document_Command(document, &Document::copy);
        copy_item->add_command_object(copy_cmd_object);
        
        edit_menu->add_menu_item(std::move(copy_item));
        
        unique_ptr<Menu_Item> paste_item = make_unique<Menu_Item>("Paste");
        Paste_Document_Command *paste_cmd_object = new Paste_Document_Command(document, &Document::paste);
        paste_item->add_command_object(paste_cmd_object);
        
        edit_menu->add_menu_item(std::move(paste_item));
        
        unique_ptr<Menu_Item> cut_item = make_unique<Menu_Item>("Cut");
        Cut_Document_Command *cut_cmd_object = new Cut_Document_Command(document, &Document::cut);
        cut_item->add_command_object(cut_cmd_object);
        
        edit_menu->add_menu_item(std::move(cut_item));
        
        
        menus.push_back(std::move(file_menu));
        menus.push_back(std::move(edit_menu));
    }
    
    int select_menu()
    {
        int choice = 0;
        int end_app = 0;
        cout << "Enter your Menu choice" << endl;
        for(auto &menu : menus)
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
            // set the trigger 
            trigger = signal::app_exit;
            end_application();
            return 1;
        }
        
        return 0;
    }
    
    void notify_doc_close()
    {
        cout << "Application : document close() detected" << endl;
        
        if(trigger != signal::app_exit) // not supporting re-opening or opening another document 
        {
            end_application();
        }
    }
    
    void end_application()
    {
        // do clean up
        // release back the memory resources to free store 
        for(auto &menu : menus)
        {
            menu.reset();
        }
        
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

