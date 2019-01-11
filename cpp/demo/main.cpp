#include <iostream>
#include <string>


/*   Module  Parameters  */
class Parameters{
public:    
    Parameters(const char * cmdName, const char * dev, const int fatType)
    :_cmdName(cmdName)
    {
    }
    ~Parameters(){
    }

    const char * getCmdName() const {
        return _cmdName;
    }

    const char * getString(const char * key){
        return NULL;
    }
    int getInt(const char * key){
        return 0;
    }
    long long getLongLong(const char * key){
        return 0l;
    }
    double getDouble(const char * key){
        return 0.0;
    }

private:
    const char * _cmdName;
};


/*  
class FatAppArgsParser : public AbstractArgsParser{
};*/



/*  Module  Cmd   */

class ICmd {
public:
    virtual ~ICmd(){}
    virtual const char * name() = 0;
    virtual int config(Parameters * params) = 0;
    virtual int execute() = 0;
};

class AbstractCmd : public ICmd{
public:
    AbstractCmd(const char * name)
    :_name(name)
    {
    }
    AbstractCmd(std::string & name)
    :_name(name)
    {
    }

    virtual ~AbstractCmd(){};

    const char * getName(){
        return _name.c_str();
    }

    virtual int config(Parameters * params){
        std::cout << "config:" << _name << std::endl;
        return 0;
    }
    virtual int execute() {
        std::cout << "execute" << _name << std::endl;
        return  0;
    }

private:
    std::string _name;
};

class PrintCmd : public AbstractCmd{
public:
    PrintCmd()
    :AbstractCmd("fat.print")
    {
    }
    virtual ~PrintCmd(){
    }

    virtual int execute(){
        AbstractCmd::execute();
        return 0;
    }
};

#include <unordered_map> //TODO: add the hashmap;  move the jcx::base into single git repo
/*  Module    PluginManager    */
template<typename T>
class PluginManager {
public:   
    PluginManager(){}
    ~PluginManager(){}

    
    static int load(const std::string & path){
        return 0;
    }

    void unload(){
    }


    void add(const std::string & name, T * plugin){
        _pluginMap.insert_or_assign(name, plugin);
    }
    void remove(const std::string & name){
        auto it = _pluginMap.find(name);
        if( it != _pluginMap.end()){
            _pluginMap.erase(it);
        }
    }

    T * getPlugin(const std::string & name){
        auto it = _pluginMap.find(name);
        if(it != _pluginMap.end()){
            return _pluginMap[name] ;
        }
        return NULL;
    }

    std::unordered_map<std::string, T*> _pluginMap;
};



void test(int argc, char * argv[]){
    auto pm = new PluginManager<ICmd>(); //load 

    
    const char * cmdName = "print";
    const char * dev = "a.img";
    const int fatType = 1;
    auto params = new Parameters(cmdName, dev, fatType);
        
    ICmd * cmd = pm->getPlugin(params->getCmdName());
    if(cmd != NULL){
        cmd->config(params);
        cmd->execute();
    }

    delete params;
    delete pm; //unload 
}



int main(int argc, char * argv[]){
    return 0;
}
