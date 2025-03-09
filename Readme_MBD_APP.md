# 🚀 Acceleration Pedal Position Modeling and Testing (MIL & SIL)

## 📂 Project Overview
This project focuses on developing a **Model-Based Design (MBD)** approach for the **Acceleration Pedal Position** system in automotive applications. The primary goal is to model the acceleration pedal position, implement fault detection control logic, and validate the system using **Model-in-the-Loop (MIL)** and **Software-in-the-Loop (SIL)** testing.

---

## 📝 Project Objectives
1. **Model the Acceleration Pedal Position:**  
   - Create mathematical models for dual sensor configurations.  
   - Simulate sensor behavior, including signal noise and failures.  

2. **Fault Detection and Control Logic:**  
   - Implement logic to detect faulty sensors and ensure robust operation.  
   - Develop algorithms to identify and isolate malfunctioning sensors.  

3. **Testing and Validation:**  
   - Perform **MIL testing** to verify the model and fault detection logic.  
   - Conduct **SIL testing** to validate auto-generated code in a virtual environment.  

---

## 🛠️ Project Structure
```
Acceleration_Pedal_Modeling/
├── Models/                   # MATLAB/Simulink models and blocks
├── ControlLogic/             # Control algorithms and fault detection logic
├── Tests/                    # MIL and SIL test cases
├── Results/                  # Test results and analysis
├── Documentation/            # Project reports and documentation
└── README.md                 # Project description and usage guide
```

---

## 💻 Getting Started

### 🔧 Prerequisites
Make sure you have the following installed:
- **MATLAB R2023a or later**
- **Simulink**
- **Embedded Coder** (for SIL testing)
- **MATLAB Coder** (for auto code generation)

### 📥 Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/username/Acceleration_Pedal_Modeling.git
   ```
2. Open MATLAB and navigate to the project folder:
   ```matlab
   cd Acceleration_Pedal_Modeling
   ```

---

## 🧪 Running the Models

### 1. **Model-in-the-Loop (MIL) Testing**
To run the MIL tests:
1. Open the **Simulink model**:
   ```matlab
   open('Models/AccelerationPedalModel.slx')
   ```
2. Run the simulation and observe the **signal consistency and fault detection**.

### 2. **Software-in-the-Loop (SIL) Testing**
To perform SIL tests:
1. Generate code from the control logic:
   ```matlab
   rtwbuild('ControlLogic/AccelerationControl')
   ```
2. Run the **SIL test script**:
   ```matlab
   run('Tests/SIL_Test.m')
   ```
3. Analyze the results using provided scripts:
   ```matlab
   analyzeSILResults('Results/SIL_Results.mat')
   ```

---

## 📝 Results and Analysis
The test results and analysis are saved in the **Results/** folder. Each test run is automatically logged with:
- **Signal consistency data**
- **Fault detection logs**
- **Performance metrics**

---

## 🤝 Acknowledgements
Special thanks to **Eng. Abdelrhman Shaban** and **OS Academy** for their invaluable support and guidance throughout this project.

---

## 📝 License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.

---

## 💬 Contact
For any questions or collaboration opportunities, feel free to reach out to me on [LinkedIn](https://www.linkedin.com/in/ahmed-mohsen-abouelyazed) or via email at **ahmedmohsen_209@hotmail.com**.

---

Let me know if you need any adjustments or additional sections! 😊
