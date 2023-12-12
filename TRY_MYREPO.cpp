#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include <vector>

#define MAXFILESIZE 50
using namespace std;



class Time {
//时间格式: hour:minute,+day
public:
    int hour;
    int minute;
    int day;

    Time (const int h=0, const int min=0, const int d=0) : hour(h), minute(min), day(d) {}
    Time (const Time &obj) : hour(obj.hour), minute(obj.minute), day(obj.day) {}

    /**
     * @brief 交换两个 Time 对象的值。
     * @param lt 左侧 Time 对象。
     * @param rt 右侧 Time 对象。
     */
    void swap (Time &lt, Time &rt) {
        std::swap (lt.day, rt.day);
        std::swap (lt.hour, rt.hour);
        std::swap (lt.minute, rt.minute);
    }

    /**
     * @brief 赋值运算符重载，使用交换技巧实现。
     * @param other_obj 要赋值的 Time 对象。
     * @return 赋值后的当前对象的引用。
     */
    Time & operator = (Time other_obj) {
        swap(*this, other_obj);
        return *this;
    }

    /**
     * @brief 获取总时间
     * @return 返回总时间
    */
    int getTotalMintue () const {
        return this->day * 1440 + this->hour * 60 + this->minute;
    }
    
};

/**
 * @brief 重载小于运算符 (<) 用于比较两个 Time 对象。
 * @param t1 左侧 Time 对象。
 * @param t2 右侧 Time 对象。
 * @return 如果 t1 小于 t2，则返回 true；否则返回 false。
 *
 * 该比较基于两个 Time 对象的总分钟数，用于在排序和比较中确定时间的先后顺序。
 */
bool operator < (Time t1, Time t2) {
    return (t1.getTotalMintue() < t2.getTotalMintue());
}

Time operator - (Time t1, Time t2){
// Time 对象的"-"重载，实现两 Time 对象做减法，默认 t2 晚于 t1 时才可以做减法
// 可以简化用getTOtalMinute
    if (t1 < t2) {
        throw runtime_error ("Time Operator - 运算中 t1 小于 t2，结果将是负数!");
    }

    Time tmp_t;
    
    tmp_t.minute = t1.minute - t2.minute;
    if (tmp_t.minute < 0) {     
        tmp_t.minute += 60;
        --t1.hour;
    }
    
    tmp_t.hour = t1.hour - t2.hour;
    if (tmp_t.hour < 0) {
        tmp_t.hour += 24;
        --t1.day;
    }
    
    tmp_t.day = t1.day - t2.day;
    
    return tmp_t;
}

istream & operator >> (istream &in, Time &time) {
//'>>' 重载，实现"hour:minute,+day" 的存储
    char c1, c2, c3;
    int hour, minute, day;
    in >> hour >> c1 >> minute >> c2 >> c3 >> day;
    time.day = day;
    time.minute = minute;
    time.hour = hour;
    return in;
} 

ostream & operator << (ostream &out, const Time &time) {
//运算符'<<'重载以美化 Time 对象的输入

    //数据之间空白用'0'填充
    cout.fill('0');

    //数据的右对齐与天数的'+'号
    out << setw(2) << setiosflags(ios_base::right) <<time.hour << ":" 
        << setw(2) << time.minute << resetiosflags(ios_base::right) 
        << "," << setiosflags(ios_base::showpos) << time.day << resetiosflags(ios_base::showpos); //setiosflags(ios_base::showpos) 表示 "+" 正号

    cout.fill(' '); //fill setiosflags 需手动关闭

    return out;
} 

//////////////////////////////////////// 以上是对Time相关操作的重载 ////////////////////////////////////////



class LineNode {  // 线路信息，作为边表的元素
public:
    std::string start_city_name; ///< 起始城市名称
    std::string end_city_name;   ///< 终点城市名称
    Time start_time;             ///< 起始时间
    Time end_time;               ///< 终点时间
    Time spend_time;             ///< 路程花费时间
    float spend_money;           ///< 花费金钱
    std::string rank;            ///< 火车或飞机的班次
    

    /**
     * @brief 构造函数，初始化 LineNode 对象。
     * @param scn 起始城市名称。
     * @param ecn 终点城市名称。
     * @param st 起始时间。
     * @param et 终点时间。
     * @param spend_t 路程花费时间。
     * @param spend_m 花费金钱。
     * @param amt 班次（火车或飞机）。
     */
    LineNode(const string scn, const string ecn, const Time st, const Time et, const Time spend_t, 
            const float spend_m, const string amt) 
            : start_city_name(scn), end_city_name(ecn), 
            start_time(st), end_time(et), spend_time(spend_t), spend_money(spend_m), rank(amt) {}

};


class Vnode {  // 顶点表中的头结点，存储始发站的信息
public:
    Vnode (const string scn, const int cid) : start_city_name(scn), city_id(cid) {}
    Vnode (const char* scn, const int cid) : start_city_name(scn), city_id(cid) {}
    Vnode (const string scn) : start_city_name(scn), city_id(-1) {}
    Vnode (const char* scn) : start_city_name(scn), city_id(-1) {}
    
    string start_city_name;  // 地名
    int city_id;
};

// class cmp_vnode {  // 定义从 Vnode 映射 vector<InfoType> 的 map 的关键字比较关系
// public:
// 	bool operator() (const Vnode& x, const Vnode& k) const {
// 			// if(x.start_city_name >= k.start_city_name)
// 			// return false;  //STL源码规定必须这么返回，而不能改成 true
// 			// else
// 			// return true;
//             return x.start_city_name < k.start_city_name;d
// 	}
// };

//运算符'<<'重载以美化 LineNode 对象的输入
ostream & operator << (ostream &out, const LineNode &line) {

    cout << "出发城市|到达城市|班次名|出发时间|到达时间|路程用时|路程票价" << endl;
    cout << setw(10) << line.start_city_name << " " <<  setw(10) << line.end_city_name 
        << " " << setw(6) << line.rank << " ";
    cout.fill('0'); // 修改 fill() 的默认填充符为 0
    cout << line.start_time << " " << line.end_time << " " << line.spend_time << " "
        << setiosflags(ios_base::showpoint) << line.spend_money << endl;
    cout.fill(' '); // fill() 不像 setw(), 需要手动调整

    return out;
} 




class ALGraph {
    public:
        auto cityGo ();  // 输出出发城市
        auto cityArrive ();  //输出到达城市
        bool ifCityExist (const string & city_name);  // 查询城市是否存在
        void addCityFromFile (const char FILENAME[MAXFILESIZE]);  // 从文件读取以添加城市
        void addLine();  // 手动添加线路
        void addLineFromFile(const char FILENAME[MAXFILESIZE]);  // 从文件读取以添加线路

        // 删除线路，需要手动输入起点与终点
        void delLine(const std::string &sc, const std::string &ec, const std::string &amt);  
        // 删除城市，并删除以该城市为起点的航班和列车信息
        void delCity(const std::string &city_name);  
        //void updateFile(const char FileName[MAXFILESIZE], const std::string type);  // 修改后更新文件

        void showAllCity();  // 输出所有城市
        void showAllLine();  // 输出所有线路

    //     // 返回从起点城市到终点城市的所有路径
    //     std::vector<std::vector<LineNode>> getPathsByCity (const std::string &sc, const std::string &ec);

    //     // 打印从起点城市到终点城市的所有路径
    //     void printPathsByCity (const std::string &sc, const std::string &ec);
        
    //     // 输出从起点城市到终点城市，中转次数最少的路径
    //     void printLeastTransferPath (const std::string &sc, const std::string &ec);  
        
    //     // 输出从起点城市到终点城市，花费最小的线路
    //     void printLeastMoneyPath (const std::string &sc, const std::string &ec);  

    //     // 输出从起点城市到终点城市，总时间最短的线路
    //     void printLeastTimePath (const std::string &sc, const std::string &ec);
        
    // private:
    //     // 定义从 Vnode 映射 vector<InfoType> 的 map, 关键字为 Vnode 中的 start_city_name，关键字的关系为 cmp_vnode
    //     std::map <Vnode, std::vector<LineNode>, cmp_vnode > m;

        int city_num;
        int line_num;

    //     // 通过起点城市、终点城市、班次，查询一条线路信息
    //     std::vector<LineNode> getLineNode (const std::string sc, const std::string ec, const std::string amt);

};


auto ALGraph::cityGo()
{
    //暂未更改路径
	std::ifstream inputFile("数据结构/National-Transport-Advisory/Flight.txt");  // 用你的文件名替换 "your_file.txt"

    if (!inputFile.is_open()) {
        std::cerr << "无法打开文件\n";
    }

    std::set<std::string> departureCities;

    std::string line;

	std::getline(inputFile, line);
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string departureCity;
        iss >> departureCity;  // 从每一行提取出发城市

        // 将城市添加到集合中
        departureCities.insert(departureCity);
    }

	return departureCities;
}

auto ALGraph::cityArrive()
{
    std::ifstream inputFile("数据结构/National-Transport-Advisory/Flight.txt"); // 请替换为实际的文件名
    if (!inputFile.is_open()) {
        std::cerr << "无法打开文件\n";
    }

    std::set<std::string> uniqueArrivalCities;

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        // 跳过出发城市和其他信息
        std::string departureCity, arrivalCity;
        iss >> departureCity >> arrivalCity; // 读取到达城市
        uniqueArrivalCities.insert(arrivalCity); // 插入到set中
    }

    inputFile.close();


    return uniqueArrivalCities;
}

/**
 * @brief 查询城市是否存在于图中。
 * @param city_name 要查询的城市名称。
 * @return 如果城市存在于图中，则返回 true；否则返回 false。
 */
bool ALGraph::ifCityExist(const string &city_name) {
    for (const auto &city : cityGo()) {
        if(city == city_name) return true;
    }
}

//手动添加线路
void ALGraph::addLine () {
    string start_city_name;
    cout << "请输入起点城市：";
    cin >> start_city_name;
    
    if (!ifCityExist(start_city_name)) {
        cout << "起点该城市并不存在，请先创建该城市！" << endl;
        return;
    }

    string end_city_name;
    Time start_time, end_time;
    Time spend_time;
    float spend_money;
    string rank;  // 火车或飞机的班次

    cout << "请输入终点城市：";
    cin >> end_city_name;
    if (!ifCityExist(end_city_name)) {
        cout << "终点城市" << end_city_name << "并不存在，请先创建该城市！" << endl;
        return;
    }
    cout << "请输入班次名：";
    cin >> rank;
    cout << "请输入出发时间(格式为hh:mm,+d)：";
    cin >> start_time;
    cout << "请输入到达时间(格式为hh:mm,+d)：";
    cin >> end_time;

    if (start_time < end_time) {
        spend_time = end_time - start_time;
    } else{
        cout << "出发时间大于到达时间，输入错误，请重新输入！" << endl;
        return;
    }

    cout << "请输入票价：";
    cin >> spend_money;

    // auto it = m.find(Vnode(start_city_name));
    // if (it != m.end()) {
    //     (*it).second.push_back(LineNode (start_city_name, end_city_name, start_time, end_time, 
    //                 spend_time, spend_money, rank));
    //     cout << "添加路线成功！" << endl;
    // }

    const char* fileName = "数据结构/National-Transport-Advisory/Flight.txt";
    std::ifstream inFile(fileName);
    std::ofstream tempFile("temp.txt"); // 临时文件

    if (!inFile.is_open() || !tempFile.is_open()) {
        std::cerr << "无法打开文件\n";
        return;
    }

    std::string line;
    bool inserted = false;

    while (std::getline(inFile, line)) {
        tempFile << line << std::endl;

        // 如果找到插入点，插入新的数据
        if (!inserted && line.find(start_city_name) != std::string::npos) {
            cout << 666666666666666666666;
            // tempFile <<  setw(10) << start_city_name << " " << setw(10) << end_city_name << " "
            //           << rank << " " << start_time << " "
            //           << end_time << " " << spend_time << " "
            //           << std::setprecision(3) << std::fixed << spend_money << std::endl;

            tempFile << setw(10) << start_city_name << " " <<  setw(10) << end_city_name 
                      << " " << setw(6) << rank << " ";
            tempFile.fill('0'); // 修改 fill() 的默认填充符为 0
            tempFile << start_time << " " << end_time << " " << spend_time << " "
                 << setiosflags(ios_base::showpoint) << spend_money << endl;
            tempFile.fill(' '); // fill() 不像 setw(), 需要手动调整
            inserted = true;
        }
    }

    // 如果数据应该插入到文件末尾
    if (!inserted) {
            tempFile << LineNode (start_city_name, end_city_name, start_time, end_time,spend_time, spend_money, rank);
    }

    inFile.close();
    tempFile.close();

    // 替换原始文件
    std::remove(fileName);
    std::rename("temp.txt", fileName);




    ++line_num;

}//addLine 

int main()
{
    //功能实现基本示例
    //Time
    Time t1;
    t1.minute=10;
    t1.hour=14;
    t1.day=0;
    Time t2;
    t2.minute=50;
    t2.hour=14;
    t2.day=1;

    cout << t1.getTotalMintue() << endl;
    Time t3 = t2 - t1;
    cout << t3.getTotalMintue() << endl;
    t2 = t1;
    cout << t2.getTotalMintue() << endl;

    //Node

    ALGraph a;
    // string start_city_name = "上海";
    // string end_city_name = "北京";
    // string rank = "AF1000";
    // Time start_time(14,20,+0);
    // Time end_time(15,50,+0);
    // Time spend_time(01,30,+0);
    // float spend_money = 380.0;
    // LineNode sampleLine(start_city_name, end_city_name, start_time, end_time, spend_time, spend_money, rank);
    // cout << sampleLine;


    a.addLine();
    return 0;
}