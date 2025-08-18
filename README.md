![Tribots banner](assets/banner.svg)

# 🤖 Tribots — WRO 2025

## Team Members
- **Ehtasham** — Electronics & Firmware  
- **Behzad** — Mechanical & 3D Design  
- **Shahim** — Software & Vision  

---

## 🛠 Components (short)
Arduino Nano / Mega · L298N · TB6612 · VL53L0X TOF · Encoders · LM2596 buck · LiPo · DC motors · Servos

---

## 📋 Bill of Materials (sample)

| Component            | Qty | Purpose                         |
|----------------------|-----|---------------------------------|
| Arduino Nano / Mega  | 1   | Microcontroller                 |
| TB6612FNG / L298N    | 1   | Motor drivers                   |
| VL53L0X (TOF)        | 2–4 | Distance sensing                |
| Wheel encoders       | 2   | Odometry                        |
| LM2596 Buck          | 1   | 5V power rail                   |
| LiPo battery         | 1   | Power source                    |

---

## 🚀 Quick notes
- Keep motors on a separate power rail from Arduino.  
- Always common ground.  
- Use branches for features: `feat/line-follow`, `chore/hardware-docs`.
