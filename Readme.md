# Single-Event Latchup (SEL) Detection for ZES ZSOM-M01

This Arduino-based program is designed for use with the **ZES ZSOM-M01** and the accompanying **ZSOM-M01-NS01A evaluation board**. It monitors and detects **Single-Event Latchup (SEL) pulses** induced by the **SEL button** attached to the board.

---

## 🔍 Overview

- The board integrates the AD7888 Analog-to-Digital Converter (ADC) as **an onboard test circuit** to demonstrate SEL events.  
- The **SEL button** can be connected across the ADC to simulate an **SEL pulse**.  
- The program detects SEL pulses and **displays the detection** via the Arduino Serial Monitor.  
- It also **displays runtime information** on the Serial Monitor.

---

## 🧪 Demo/Test Instructions

1. Connect the **SEL button** as shown in the image and power on the board.  
2. Wait for the **"Startup done!"** message on the Arduino Serial Monitor.  
3. **Press** the button to generate an SEL pulse.  
4. A **"SEL detected!"** message will appear on the Serial Monitor.  
5. Press the button again to reset it before generating another SEL event.

---

## 📁 Repository Contents

- `SEL_Detection/` — Arduino sketch files  
- `images/` — Board images (ZES ZSOM-M01 + ZSOM-M01-NS01A evaluation board)  
- `README.md` — This documentation  
- `LICENSE.txt` — License file  

---

## 🛠️ Requirements

- ZES ZSOM-M01 board  
- ZSOM-M01-NS01A evaluation board  
- Arduino IDE  

---

## 📜 License

This project is licensed under the [MIT License](LICENSE).

---

## ✍️ Author

- Zero-Error Systems Pte. Ltd.
