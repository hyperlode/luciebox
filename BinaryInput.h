
#ifndef BinaryInput_h
#define BinaryInput_h

class BinaryInput{

  public:
    void setValue(bool value);
    bool getValue();
    bool getEdgeDown();
    bool getEdgeUp();
    bool getValueChanged();
    void refresh();
    
  private:
    bool previousValue;
    bool value;
  
};

#endif


