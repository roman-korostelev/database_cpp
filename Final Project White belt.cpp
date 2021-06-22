#include "iostream"
#include "map"
#include "set"
#include "string"
#include "sstream"
#include "iomanip"
#include "math.h"
#include "algorithm"
using namespace std;

void skipSymbol(stringstream& stream)
{
    if (stream.peek() != '-')
    {
        stringstream ss;
        ss << "Wrong date format: " << stream.str();
        throw runtime_error(ss.str());
    }
    stream.ignore(1);
}
class Date {
private:
    int day = 0;
    int month = 0;
    int year = 0;
public:
    Date(const string& s)
    {
        try
        {

            string rdate;
            rdate.resize(s.size());
            reverse_copy(s.begin(), s.end(), rdate.begin());
            rdate += " ";
            int powr = 0,pos = 0;
            if (rdate[pos] == '-')
            {
                throw runtime_error("Wrong date format: " + s);
            }
            while(rdate[pos] != '-')
            {
                if (rdate[pos] >= '0' && rdate[pos] <= '9')
                {
                    day += (rdate[pos] - '0')*pow(10,powr++);
                }
                else if (rdate[pos] == '+')
                {
                    pos++;
                    continue;
                }
                else
                {
                    throw runtime_error("Wrong date format: " + s);
                }
                pos++;
            }
            if (rdate[pos + 1] == '-')
            {
                day = -day;
                pos++;
            }
            powr = 0;
            while (rdate[++pos] != '-')
            {
                if (rdate[pos] >= '0' && rdate[pos] <= '9')
                {
                    month += (rdate[pos] - '0')*pow(10,powr++);
                }
                else if (rdate[pos] == '+')
                {
                    continue;
                }
                else
                {
                    throw runtime_error("Wrong date format: " + s);
                }
            }
            if (rdate[pos + 1] ==  '-')
            {
                month = -month;
                pos++;
            }
            powr = 0;
            while (rdate[++pos] != ' ' && rdate[pos] != '-')
            {
                if (rdate[pos] >= '0' && rdate[pos] <= '9')
                {
                    year += (rdate[pos] - '0')*pow(10,powr++);
                }
                else if (rdate[pos] == '+')
                {
                    continue;
                }
                else
                {
                    throw runtime_error("Wrong date format: " + s);
                }
            }
            if (rdate[pos] == '-')
            {
                year = -year;
            }
            int len = s.length();
            if(len != pos + year > 0 ? 0: 1)
            {
                throw runtime_error("Wrong date format: " + s);
            }
            if (month > 12 || month < 1)
            {
                throw runtime_error("Month value is invalid: " + to_string(month));
            }
            if (day > 31 || day < 1)
            {
                throw runtime_error("Day value is invalid: " + to_string(day));
            }
        }
        catch (exception& ex)
        {
            cout << ex.what();
            exit(0);
        }
    }
    void PrintDate() const
    {
        cout << setw(4) << setfill('0') << year << '-' << setw(2) << setfill('0') << month << '-' << setw(2) << setfill('0') << day;
    }
    int GetYear() const
    {
        return year;
    }
    int GetMonth() const
    {
        return month;
    }
    int GetDay() const
    {
        return day;
    }
};
bool operator<(const Date& lhs, const Date& rhs)
{
    if (lhs.GetYear() != rhs.GetYear())
    {
        return lhs.GetYear() < rhs.GetYear();
    }
    if (lhs.GetMonth() != rhs.GetMonth())
    {
        return lhs.GetMonth() < rhs.GetMonth();
    }
    return lhs.GetDay() < rhs.GetDay();
}

class Database {
private:
    map<Date, set<string>> base;
public:
    void AddEvent(const Date& date, const string& event)
    {
        base[date].insert(event);
    }
    bool DeleteEvent(const Date& date, const string& event)
    {
        return base[date].erase(event);
    }
    int  DeleteDate(const Date& date)
    {
        int temp = base[date].size();
        base[date].clear();
        return temp;
    }
    void Find(const Date& date) const
    {
        try
        {
            for (auto& event : base.at(date))
            {
                cout << event << endl;
            }
        }
        catch (exception&)
        {
            cout << "";
        }
    }
    void Print() const
    {
        for (auto& [date, events] : base)
        {
            if (events.empty())
            {
                continue;
            }
            for (auto& event : events)
            {
                date.PrintDate();
                cout <<  ' ' << event << endl;
            }
        }
    }
};

int main() {
    Database db;

    string line;
    while (getline(cin, line)) {
        stringstream stream(line);
        string command;
        stream >> command;
        if (command =="Add")
        {
            string date, event;
            try
            {
                stream >> date >> event;
                db.AddEvent({date}, event);
            }
            catch (exception& ex)
            {
                cout << ex.what() << endl;

            }
        }
        else if (command == "Del")
        {
            string date, event;
            try
            {
                stream >> date >> event;
                if (event.size())
                {
                    db.DeleteEvent({date}, event) ? cout << "Deleted successfully" << endl : cout <<  "Event not found" << endl;
                }
                else
                {
                    int n = db.DeleteDate({date});
                    cout << "Deleted " << n << " events" << endl;
                }
            }
            catch (exception& ex)
            {
                cout << ex.what() << endl;
            }
        }
        else if (command == "Find")
        {
            string date;
            stream >> date;
            db.Find({date});
        }
        else if (command == "Print")
        {
            db.Print();
        }
        else if (command.size())
        {
            cout << "Unknown command: " << command << endl;
        }
    }

    return 0;
}