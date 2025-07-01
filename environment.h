#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <utility>

using namespace std;

class Environment
{
private:
    vector<unordered_map<string, float>> levels;       // Almacena valores de variables
    vector<unordered_map<string, string>> type_levels; // Almacena tipos de variables

    unordered_map<string, unordered_map<string, double>> record_values;
    unordered_map<string, unordered_map<string, string>> record_types;
    unordered_map<string, vector<pair<string, string>>> struct_defs;

    // Busca el nivel en el que está una variable
    int search_rib(string var)
    {
        int idx = levels.size() - 1;
        while (idx >= 0)
        {
            if (levels[idx].find(var) != levels[idx].end())
            {
                return idx;
            }
            idx--;
        }
        return -1;
    }

public:
    Environment() {}

    void clear()
    {
        levels.clear();
        type_levels.clear();
        record_values.clear();
        record_types.clear();
        struct_defs.clear();
    }

    void add_level()
    {
        levels.emplace_back();
        type_levels.emplace_back();
    }

    void add_var(string var, float value, string type)
    {
        if (levels.empty())
        {
            cerr << "Environment sin niveles: no se pueden agregar variables" << endl;
            exit(1);
        }
        levels.back()[var] = value;
        type_levels.back()[var] = type;
    }

    bool add_var(string var, string type)
    {
        if (levels.empty())
        {
            cerr << "Environment sin niveles: no se pueden agregar variables" << endl;
            exit(1);
        }

        if (levels.back().count(var))
        {
            return false;
        }

        levels.back()[var] = 0;
        type_levels.back()[var] = type;
        return true;
    }

    bool remove_level()
    {
        if (!levels.empty())
        {
            levels.pop_back();
            type_levels.pop_back();
            return true;
        }
        return false;
    }

    bool update(string x, float v)
    {
        int idx = search_rib(x);
        if (idx < 0)
            return false;
        levels[idx][x] = v;
        return true;
    }

    bool check(string x)
    {
        return search_rib(x) >= 0;
    }

    float lookup(string x)
    {
        int idx = search_rib(x);
        if (idx < 0)
        {
            cerr << "Variable no declarada: " << x << endl;
            exit(1);
        }
        return levels[idx][x];
    }

    string lookup_type(string x)
    {
        int idx = search_rib(x);
        if (idx < 0)
        {
            cerr << "Variable no declarada: " << x << endl;
            exit(1);
        }
        return type_levels[idx][x];
    }

    bool typecheck(string var, string expected_type)
    {
        string actual_type = lookup_type(var);
        if (actual_type != expected_type)
        {
            cerr << "Error de tipo: se esperaba " << expected_type
                 << " pero se encontró " << actual_type
                 << " para la variable " << var << endl;
            return false;
        }
        return true;
    }

    //----- STRUCTS -----//
    void add_struct(const string &name, const vector<pair<string, string>> &members)
    {
        if (struct_defs.count(name))
        {
            cerr << "Struct ya definido: " << name << endl;
            exit(1);
        }
        struct_defs[name] = members;
    }

    bool has_struct(const string &name) const
    {
        return struct_defs.find(name) != struct_defs.end();
    }

    bool has_field(const string &structVar, const string &field)
    {
        if (!has_struct(structVar))
        {
            return false;
        }

        for (const auto &member : struct_defs[structVar])
        {
            if (member.second == field)
            {
                return true;
            }
        }
        return false;
    }

    string get_field_type(const string &structVar, const string &field)
    {
        if (!has_struct(structVar))
        {
            cerr << "Struct no definido: " << structVar << endl;
            exit(1);
        }

        for (const auto &member : struct_defs[structVar])
        {
            if (member.second == field)
            {
                return member.first;
            }
        }

        cerr << "Campo no encontrado: " << structVar << "." << field << endl;
        exit(1);
    }

    void create_record_instance(const string &varName, const string &structType)
    {
        if (!has_struct(structType))
        {
            cerr << "No se puede crear instancia de struct no definido: " << structType << endl;
            exit(1);
        }

        record_values[varName] = {};
        record_types[varName] = {};

        for (const auto &member : struct_defs[structType])
        {
            record_values[varName][member.second] = 0;
            record_types[varName][member.second] = member.first;
        }
    }

    void set_field(string record, string field, double value)
    {
        if (record_values.find(record) == record_values.end() ||
            record_values[record].find(field) == record_values[record].end())
        {
            cerr << "Campo no encontrado: " << record << "." << field << endl;
            exit(1);
        }
        record_values[record][field] = value;
    }

    double get_field_value(string record, string field)
    {
        if (record_values.find(record) == record_values.end() ||
            record_values[record].find(field) == record_values[record].end())
        {
            cerr << "Campo no encontrado: " << record << "." << field << endl;
            exit(1);
        }
        return record_values[record][field];
    }

    void debug_print()
    {
        cout << "---- ENTORNO ACTUAL ----\n";
        for (int i = levels.size() - 1; i >= 0; i--)
        {
            cout << "Nivel " << i << ":\n";
            for (auto &[var, val] : levels[i])
                cout << "  " << var << " = " << val << " (" << type_levels[i][var] << ")\n";
        }
        cout << "\n--- STRUCT DEFINITIONS ---\n";
        for (auto &[name, members] : struct_defs)
        {
            cout << "struct " << name << " {\n";
            for (auto &[type, field] : members)
                cout << "  " << type << " " << field << ";\n";
            cout << "}\n";
        }
        cout << "\n--- RECORD INSTANCES ---\n";
        for (auto &[record, fields] : record_values)
        {
            cout << record << ":\n";
            for (auto &[f, v] : fields)
                cout << "  ." << f << " = " << v << " (" << record_types[record][f] << ")\n";
        }
    }
};

#endif // ENVIRONMENT_H