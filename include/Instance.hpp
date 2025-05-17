#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <vector>
#include <string>
#include <iostream>

using namespace std;

namespace opthirrygated {
    class Instance {
    private:

        vector<float> cicle;
        vector<int> perc;
        vector<float> cost;
        vector<float> lamp;
        vector<float> prec;
        vector<float> etc;
        vector<float> cad;
        vector<float> lc;
        string path;

    public:
        // Constructor
        Instance(string path);

        virtual void exec();

        // Getter and Setter for cicle
        vector<float> getCicle() const { return cicle; }
        void setCicle(const vector<float>& newCicle) { cicle = newCicle; }

        // Getter and Setter for perc
        vector<int> getPerc() const { return perc; }
        void setPerc(const vector<int>& newPerc) { perc = newPerc; }

        // Getter and Setter for cost
        vector<float> getCost() const { return cost; }
        void setCost(const vector<float>& newCost) { cost = newCost; }

        // Getter and Setter for lamp
        vector<float> getLamp() const { return lamp; }
        void setLamp(const vector<float>& newLamp) { lamp = newLamp; }

        // Getter and Setter for prec
        vector<float> getPrec() const { return prec; }
        void setPrec(const vector<float>& newPrec) { prec = newPrec; }

        // Getter and Setter for etc
        vector<float> getEtc() const { return etc; }
        void setEtc(const vector<float>& newEtc) { etc = newEtc; }

        // Getter and Setter for cad
        vector<float> getCad() const { return cad; }
        void setCad(const vector<float>& newCad) { cad = newCad; }

        // Getter and Setter for lc
        vector<float> getLc() const { return lc; }
        void setLc(const vector<float>& newLc) { lc = newLc; }

        // Getter and Setter for path
        string getPath() const { return path; }
        void setPath(const string& newPath) { path = newPath; }
    };
}

#endif
