#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

class Sensor {
public:
    virtual double getValue() = 0;
    virtual void updateValue(double newValue) = 0;
    virtual void tamper() = 0;
};

class MassSensor : public Sensor {
private:
    double value;
    bool isWorking;

public:
    MassSensor() : value(0), isWorking(true) {}

    virtual double getValue() override {
        if (isWorking) {
            return value;
        }
        else {
            return value; 
        }
    }

    virtual void updateValue(double newValue) {
        value = newValue;
    }

    virtual void tamper() {
        isWorking = false;
    }
};

class VolumeSensor : public Sensor {
private:
    double value;
    bool isWorking;

public:
    VolumeSensor() : value(0), isWorking(true) {}

    virtual double getValue() override {
        if (isWorking) {
            return value;
        }
        else {
            return value; 
        }
    }

    virtual void updateValue(double newValue) {
        value = newValue;
    }

    virtual void tamper() {
        isWorking = false;
    }
};

class TemperatureSensor : public Sensor {
private:
    double value;
    bool isWorking;

public:
    TemperatureSensor() : value(0), isWorking(true) {}

    virtual double getValue() override {
        if (isWorking) {
            return value;
        }
        else {
            return value; 
        }
    }

    virtual void updateValue(double newValue) {
        value = newValue;
    }

    virtual void tamper() {
        isWorking = false;
    }
};

class Tank {
private:
    double maxVolume;
    double currentVolume;
    double currentMass;
    double currentTemperature;
    double currentDensity;
    double fillRate;

    MassSensor massSensor;
    VolumeSensor volumeSensor;
    TemperatureSensor temperatureSensor;

public:
    Tank(double maxVolume, double fillRate)
        : maxVolume(maxVolume), currentVolume(0), currentMass(0), currentTemperature(0), currentDensity(0), fillRate(fillRate) {}

    void addContent(double mass, double volume) {
        if (currentVolume == maxVolume) {
            std::cout << "Резервуар полностью заполнен. Невозможно добавить содержимое." << std::endl;
            return;
        }

        double remainingVolume = maxVolume - currentVolume;

        while (volume > 0 && remainingVolume > 0) {
            double actualVolumeToAdd = std::min(volume, fillRate);
            double actualMassToAdd = mass;

            currentMass += actualMassToAdd;
            currentVolume += actualVolumeToAdd;

            volume -= actualVolumeToAdd;
            remainingVolume -= actualVolumeToAdd;

            calculateDensity();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    void removeContent(double volume) {
        if (currentVolume == 0) {
            std::cout << "Резервуар пуст. Невозможно изъять содержимое." << std::endl;
            return;
        }

        double contentToRemove = std::min(volume, currentVolume);
        double removalPercentage = contentToRemove / currentVolume;

        while (contentToRemove > 0 && currentVolume > 0) {
            double actualContentToRemove = std::min(contentToRemove, currentVolume);
            double massToRemove = actualContentToRemove * currentMass / currentVolume;

            currentMass -= massToRemove;
            currentVolume -= actualContentToRemove;

            contentToRemove -= actualContentToRemove;

            calculateDensity();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void calculateDensity() {
        if (currentVolume != 0) {
            currentDensity = currentMass / currentVolume;
        }
        else {
            currentDensity = 0;
        }
    }

    void printTankInfo() {
        std::cout << "Информация о резервуаре:" << std::endl;
        std::cout << "Текущий объем: " << currentVolume << std::endl;
        std::cout << "Текущая масса: " << currentMass << std::endl;
        std::cout << "Текущая плотность: " << currentDensity << std::endl;
        std::cout << "Текущая температура: " << currentTemperature << std::endl;
    }

    void sensorFailure(int sensorType, int failureType) {
        Sensor* sensor = nullptr;

        switch (sensorType) {
        case 1:
            sensor = &massSensor;
            break;
        case 2:
            sensor = &volumeSensor;
            break;
        case 3:
            sensor = &temperatureSensor;
            break;
        default:
            std::cout << "Неверно указан тип датчика." << std::endl;
            return;
        }

        switch (failureType) {
        case 1:
            sensor->tamper();
            std::cout << "Датчик перестал отвечать." << std::endl;
            break;
        case 2:
            sensor->tamper();
            std::cout << "Датчик начал принимать некорректные значения." << std::endl;
            break;
        case 3:
            std::cout << "Значение датчика перестает изменяться." << std::endl;
            break;
        default:
            std::cout << "Неверно указан тип поломки." << std::endl;
            break;
        }
    }
};

class System {
private:
    Tank tank;

public:
    System(double maxVolume)
        : tank(maxVolume, 1.0) {}

    void run() {
        int choice;
        while (true)
        {
            do {
                std::cout << "Выберите действие:" << std::endl;
                std::cout << "1. Добавить содержимое" << std::endl;
                std::cout << "2. Изъять содержимое" << std::endl;
                std::cout << "3. Вывести информацию о резервуаре" << std::endl;
                std::cout << "4. Выход датчика из строя" << std::endl;
                std::cin >> choice;

                switch (choice) {
                case 1: {
                    double mass, volume;
                    std::cout << "Введите массу: ";
                    std::cin >> mass;
                    std::cout << "Введите объем: ";
                    std::cin >> volume;
                    tank.addContent(mass, volume);
                    break;
                }
                case 2: {
                    double volume;
                    std::cout << "Введите объем: ";
                    std::cin >> volume;
                    tank.removeContent(volume);
                    break;
                }
                case 3:
                    tank.printTankInfo();
                    break;
                case 4: {
                    int sensorType, failureType;
                    std::cout << "Выберите тип датчика (1 - масса, 2 - объем, 3 - температура): ";
                    std::cin >> sensorType;
                    std::cout << "Выберите тип поломки (1 - датчик перестает отвечать, 2 - датчик начинает принимать некорректные значения, 3 - значение датчика перестает изменяться): ";
                    std::cin >> failureType;
                    tank.sensorFailure(sensorType, failureType);
                    break;
                }
                default:
                    break;
                }

                std::cout << std::endl;

            } while (choice != 0);
        }
    }
};

int main() {
    double maxVolume;
    std::cout << "Введите максимальный объем резервуара: ";
    std::cin >> maxVolume;

    System system(maxVolume);
    system.run();

    return 0;
}