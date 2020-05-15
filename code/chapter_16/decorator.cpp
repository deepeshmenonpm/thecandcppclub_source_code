#include <iostream>
#include <memory>

using namespace std;

class Abstract_Window
{
public:
    virtual ~Abstract_Window() {}
    virtual void draw() = 0;
    virtual void close() =0;
};

// some utility functions
int minimize_object(Abstract_Window *abs_window)
{
    // do something to minimize this window 
    return 0;
}

int restore_object(Abstract_Window *abs_window)
{
    // do something to restore this window 
    return 0;
}

class Basic_Window : public Abstract_Window
{
private:
    int x_;
    int y_;
    float width_;
    float height_;
public:
    Basic_Window(int x = 0, int y = 0, float width = 5, float height = 2.5) :
        x_{x}, y_{y}, width_{width}, height_{height}
    {
        cout << "Basic_Window instance @ (" << x_ << ", " << y_ << ")"
             << " with width " << width_ << " and height " << height_ << " is created " << endl;
    }
    
    ~Basic_Window()
    {
        cout << "Basic_Window has been destroyed" << endl;
    }
    
    void draw()
    {
        cout << "Window with [X]" << endl;
    }
    
    void close()
    {
        cout << "Basic_Window has been closed" << endl;
    }
    
};

class Window_Decorator : public Abstract_Window
{
private:
    unique_ptr<Abstract_Window> abs_window;
public:
    Window_Decorator(Abstract_Window *this_window)
    {
        abs_window.reset(std::move(this_window));
    }
    
    virtual ~Window_Decorator()
    {
        abs_window.reset();
    }
    
    void draw()
    {
        cout << "Window with [-]" << endl;
        abs_window->draw();
    }
    
    void close()
    {
        abs_window->close();
    }
};

class Minimizable_Window : public Window_Decorator
{
private:
    Abstract_Window* abs_window_;
public:
    Minimizable_Window(Abstract_Window *abs_window) : 
        Window_Decorator(abs_window),
        abs_window_{abs_window}
    {
        
    }
    
    void draw()
    {
        Window_Decorator::draw();
    }
    
    void close()
    {
        Window_Decorator::close();
    }
    
    void minimize()
    {
        if(0 == minimize_object(abs_window_)) 
        {
            cout << "Window minimized" << endl;
        }
    }
    
    void restore()
    {
        if(0 == restore_object(abs_window_))
        {
            cout << "Window restored" << endl;
        }
    }
    
    ~Minimizable_Window()
    {
        cout << "Minimizable_Window destroyed" << endl;
    }
};

class Window_Guard
{
public:
    enum class window_types{basic, minimizable};
    enum class window_operations{draw, close, minimize, restore};
private:
    unique_ptr<Abstract_Window> window;
    bool active_window;
    window_types type_;
public:
    Window_Guard(     window_types type = window_types::basic, 
                    int x = 0, 
                    int y = 0, 
                    float width = 5.0F, 
                    float height = 2.5F    ) : 
        window{nullptr}, active_window{false}, type_{type}
    {
        switch(type)
        {
            case window_types::basic:
            {
                window.reset(new Basic_Window(x, y, width, height));
                break;
            }
            case window_types::minimizable:
            {
                window.reset(new Minimizable_Window(new Basic_Window(x, y, width, height)));
                break;
            }
        }
        
        active_window = true;
    }
    
    int do_this(window_operations op)
    {
        int ret = -1;
        if(true == active_window)
        {
            switch (op)
            {
                case window_operations::draw:
                {
                    window->draw();
                    ret = 0;
                    break;
                }
                case window_operations::close:
                {
                    window->close();
                    ret = 0;
                    // destroy the window, as it is closed
                    active_window = false;
                    window.reset();
                    break;
                }
                case window_operations::minimize:
                {
                    ret = -2; // unsupported operation
                    if(type_ == window_types::minimizable)
                    {
                        // use dynamic cast 
                        auto window_ptr = dynamic_cast<Minimizable_Window*>(window.get());
                        if(window_ptr != nullptr)
                        {
                            window_ptr->minimize();
                            ret = 0;
                        }
                        
                        break;
                    }
                }
                case window_operations::restore:
                {
                    ret = -2; // unsupported operation
                    if(type_ == window_types::minimizable)
                    {
                        // use dynamic cast 
                        auto window_ptr = dynamic_cast<Minimizable_Window*>(window.get());
                        if(window_ptr != nullptr)
                        {
                            window_ptr->restore();
                            ret = 0;
                        }
                    }
                }
                default:
                {
                    // do nothing 
                }
            } // switch ends
        }
        
        return ret;
    }
    
    ~Window_Guard()
    {
        // destroy the window
        if(active_window == true)
        {
            window.reset();
        }
    }
};

int main()
{
    // create a Minimizable_Window
    Window_Guard wg_1(Window_Guard::window_types::minimizable); // use default parameters
    
    wg_1.do_this(Window_Guard::window_operations::draw);
    wg_1.do_this(Window_Guard::window_operations::minimize);
    wg_1.do_this(Window_Guard::window_operations::restore);
    wg_1.do_this(Window_Guard::window_operations::close);
    
    cout << "Main Ends here" << endl;
    
    return 0;
}

