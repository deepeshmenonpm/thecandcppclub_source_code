#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

class formatter
{
public:
	virtual ~formatter(){}
	
	void format(fstream &output_file)
	{
		string line;
		
		cout << "Start entering your text" << endl;
		cout << "Enter 'END' for ending the session" << endl;
		
		while(output_file)
		{
			getline(cin, line);
			
			if(line.compare("END") == 0)
			{
				break;
			}
			
			align(line);
			
			output_file << line << endl;
			line[0] = '\0';
		}
	}
private:
	virtual void align(string &) = 0;
};

class align_right_formatter : public formatter
{
private:
	int width_;
public:
	align_right_formatter(int width = 20) : width_{width}
	{
		
	}
private:
	void align(string & line) override
	{
		int len = line.length();
		const string space = " ";
		const string new_line = "\n";
		
		vector <string> line_lets;
		
		int rem_len = len;
		int pos = 0;
		int count = 1;
		int offset = 0;
		int org = 0;
		
		if(rem_len <= width_)
		{
			line_lets.push_back(line);
		}
		
		while(rem_len > width_)
		{
			offset = width_ * count;
			while(true)
			{
				if(line[offset] == ' ')
				{
					if(offset - org <= width_)
					{
						break;
					}
					else if (offset - 1 < 0) 
					{
						break;
					}
					
					// The string is still lengthy
					// can't break here 
					
				}
				--offset;
			}
			
			line.insert(offset, new_line);
			len = line.length();
			rem_len = (len - offset) + 1;
			org = len - rem_len;
			count++;
		}
		
		stringstream ss(line);
		string tmp;
		
		while(getline(ss, tmp, '\n'))
		{
			line_lets.push_back(tmp);
		}
		
		line.clear();
		
		for(auto str : line_lets)
		{
			int limit = width_ - str.length() - 1;
			
			for(int i = 0; i < limit; ++i)
			{
				str.insert(i, space);
			}
			
			line.append(str);
			line.append(new_line);
		}
		
		return;
	}

};

class align_left_formatter : public formatter
{
private:
	int width_;
public:
	align_left_formatter(int width = 20) : width_{width}
	{
		// do nothing
	}
private:
	void align(string & line) override
	{
		int count = 1;
		int offset = 0;
		int len = line.length();
		const string new_line{"\n"};
		int rem_len = len;
		int org = 0;
		
		while(rem_len >= width_)
		{
			offset = width_ * count;
			
			while(true)
			{
				if(line[offset] == ' ')
				{
					if(offset - org <= width_)
					{
						break;
					}
					else if (offset - 1 < 0) 
					{
						break;
					}
					
					// The string is still lengthy
					// can't break here 
					
				}
				--offset;
			}
			
			line.insert(offset + 1, new_line);
			len = line.length();
			rem_len = (len - offset) + 1;
			org = len - rem_len;
			count++;
		}
		
		return;
	}

};

class Command
{
public:
    virtual ~Command(){}
    virtual void execute() = 0;
};

class Selectable
{
public:
    virtual ~Selectable(){}
    virtual string get_name() = 0;
    virtual void select() = 0;    
};

class Menu_Item : public Selectable
{
private:
    string name_;
    unique_ptr<Command> command_object_;
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
    
    void select() override
    {
        click();
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
    
    string get_name()
    {
        return name_;
    }
};

class Menu : public Selectable
{
private:
    string name_;
    vector<unique_ptr<Selectable>> menu_items;
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
    
    void add_menu_item(unique_ptr<Selectable> menu_item)
    {
        menu_items.push_back(std::move(menu_item));
    }
    
    void select() override 
    {
        hover();
    }
    
    void hover()
    {
        int choice;
        int count = 0;
        
        for (auto &item : menu_items)
        {
            cout << item->get_name() << " : " << count++ << endl;
        }
        
        cout << "Enter your choice : ";
        cin >> choice;
        
        if(   (choice >= 0) 
            &&(choice <= menu_items.size()) )
        {
            menu_items[choice]->select();
            return;
        }
        
    }
    
    string get_name()
    {
        return name_;
    }
    
};

class Subject;

enum class doc_states{created = 0, opened, closed, changed, moved};

class Observer
{
public:
    ~Observer(){}
    virtual void update(doc_states current_state) = 0;
};

class Subject
{
public:
    ~Subject(){}
    virtual void notify() = 0;
    virtual void attach(Observer *this_observer) = 0;
    virtual void dettach(Observer *this_observer) = 0;
};

class Document : public Subject 
{
private:
    string name_;
    doc_states current_state;
    vector<Observer*> observers; 
	fstream file;
public:
    typedef void (Document::*doc_function)();
    Document(): current_state{doc_states::created}
    { 
        /* do nothing */ 
    }
    
    ~Document()
    {
        if(   (current_state != doc_states::closed)
            &&(current_state != doc_states::created) )
        {
            // i.e. a document is open. So close it.
            close();
        }
    }
    
    void open_read()
    {
        cout << "Enter the file name to open for reading" << endl;
        cin >> name_;
		
		
		file.open(name_, ios::in); // read only mode 
		
		if(file)
		{
			current_state = doc_states::opened;
			cout << "Document : " << name_ << " is open for reading!\n" << endl;
			string line;
			
			while(file >> line)
			{
				cout << line << endl;			
			}
			
			cout << "\nEnd of File!" << endl;
			
		}
    }
    
    void open_write()
    {
        cout << "Enter the file name to open for writing" << endl;
        cin >> name_;
        
		file.open(name_, ios::app); // write in append mode  
		formatter *frmtr = nullptr;
		
		if(file)
		{
			file << "\n" << endl;
			current_state = doc_states::opened;
			cout << "Document : " << name_ << " is open for writing" << endl;
			// start writing
			cout << "Select alignment : Left(0), Right(1)" << endl;
			
			int alignment_selected = 0;
			int width = 0;
			
			cin >> alignment_selected;
			
			cout << "Select width : " << endl;
			cin >> width;
			if( (width < 0) || (width > 30) )
			{
				width = 20;
			}
				
			if(alignment_selected == 1)
			{
				// right 
				frmtr = new align_right_formatter(width);
			}
			else
			{
				// left 
				frmtr = new align_left_formatter(width);
			}
			
			if(frmtr != nullptr)
			{
				frmtr->format(file);
				delete frmtr;
			}
			
		}
    }
    
    void close()
    {
       if(   (current_state != doc_states::closed)
            &&(current_state != doc_states::created) )
        {
            file.close();
			current_state = doc_states::closed;
            cout << "Document : " << name_ << " is closed" << endl;
            name_.clear();
            // notify the subscribed observers 
            notify();
        }
        else
        {
            cout << "No documents are open" << endl;
        }
    }
    
    //void copy(int start, int end, string &copied_text)
    void copy()
    {
        if(current_state != doc_states::closed)
        {
            cout << "copying..." << endl;
            // copy from start to end to copied_text
            cout << "copy complete" << endl;
        }
        else
        {
            cout << "No documents are open" << endl;
        }
    }
    
    //void paste(int position, string& insert_text)
    void paste()
    {
        if(current_state != doc_states::closed)
        {
            cout << "pasting..." << endl;
            cout << "pasting complete" << endl;
        }
        else
        {
            cout << "No documents are open" << endl;
        }
    }
    
    //void cut(int start, int end, string &extracted_text)
    void cut()
    {
        if(current_state != doc_states::closed)
        {
            cout << "cutting..." << endl;
            // copy from start to end to copied_text
            cout << "cut complete" << endl;
        }
        else
        {
            cout << "No documents are open" << endl;
        }
    }
    
    // support for Observer pattern
    void notify() override
    {
        for( auto &observer : observers )
        {
            observer->update(current_state);
        }
    }
    
    void attach(Observer *this_observer) override
    {
        observers.push_back(this_observer);
    }
    
    void dettach(Observer *this_observer) override
    {
        int position = 0;
        auto begin = observers.begin();
        auto end = observers.end();
        
        for( auto itr = begin; itr < end; ++itr )
        {
            auto obs = *itr;
            if(this_observer == obs)
            {
                observers.erase(itr);
            }
        }
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


class Application : public Observer
{
private:
    vector<unique_ptr<Menu>> menus;
    shared_ptr<Document> current_doc;
    enum class signal{app_launched = 0, app_exit, user_interrupt, other, no_trigger};
    signal trigger;
    
    void prompt_to_open_new_document()
    {
        // prompt the user 
        cout << "Open a document for editing" << endl;        
        return;
    }
    
public:
    Application()
    {
        string doc_name;
        trigger = signal::app_launched;
        
        cout << "Document Editor App, Version : 3.0" << endl;
        
        shared_ptr<Document> document = make_shared<Document>();
        current_doc = document;

        // Attach this 'Application' as an Observer of document 
        (document.get())->attach(this);
        
        // add Menus "File" and "Edit"
        unique_ptr<Menu> file_menu = make_unique<Menu>("File");
        unique_ptr<Menu> open_sub_menu = make_unique<Menu>("Open");
        unique_ptr<Menu_Item> open_menu_item_1 = make_unique<Menu_Item>("Open For Read");
        unique_ptr<Menu_Item> open_menu_item_2 = make_unique<Menu_Item>("Open For Write");
        
        
        Open_Document_Command *open_read_cmd_object = new Open_Document_Command(document, &Document::open_read);
        Open_Document_Command *open_write_cmd_object = new Open_Document_Command(document, &Document::open_write);
        open_menu_item_1->add_command_object(open_read_cmd_object);
        open_menu_item_2->add_command_object(open_write_cmd_object);
        
        open_sub_menu->add_menu_item(std::move(open_menu_item_1));
        open_sub_menu->add_menu_item(std::move(open_menu_item_2));
        
        file_menu->add_menu_item(std::move(open_sub_menu));
        
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
        cout << "Enter your choice : " << endl;
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
            menus[choice]->select();
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
    // To support Observer pattern
    void update(doc_states current_state)
    {
        cout << "Application : document close() detected" << endl;
        switch(current_state)
        {
            case (doc_states::closed):
            {
                // open new document for editing by prompting the user
                if(trigger != signal::app_exit)
                {
                    prompt_to_open_new_document();
                }
                break;
            }
            default:
            {
                // do nothing
                break;
            }
        }
        return;
    }
    
    void end_application()
    {
        // do clean up
        // release back the memory resources to free store 
        for(auto &menu : menus)
        {
            menu.reset();
        }
        // close the current_doc
        current_doc.reset();
        
        cout << "Application exiting..." << endl;
        return;
    }
    
    ~Application()
    {
        cout << "Inside Application destructor" << endl;
    }
    
};

void app_thread_function(void)
{
    Application new_app;
    int end_app = 0;
    
    while(end_app == 0)
    {
        end_app = new_app.select_menu();
    }
    
    return;
}

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

