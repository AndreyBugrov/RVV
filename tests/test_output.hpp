class TestOutput{
protected:
    bool passed_;
    TestOutput(){}
    
public:
    TestOutput(bool passed): passed_(passed){}
    bool passed();
};