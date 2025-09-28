# 2D Verlet Physics Simulator

**A 2-month project: a 2D particle-based physics simulator implemented with Verlet Integration, rendered with Dear ImGui + DirectX11.**

This project was built to learn physics engines and level up C++ skills. It features a full ImGui control panel (open with `Insert`) for live tuning, a spatial grid for efficient collision detection, and grid partitioning to enable multithreading.

---

## 🎯 Features

* **Verlet Integration** for stable constraint solving and particle-based simulation.
* **Real-time ImGui control panel** (open with `Insert`) with many options for gravity, elasticity, shapes, and debug visuals.
* **Spatial grid collision detection** — collisions checked only when 2+ balls share the same grid cell.
* **Grid partitioning** to split work for multithreading (collision/resolution).
* **Resizable window** — particles adapt to new borders.
* **High performance** — supports roughly **~10k particles** at **~60 FPS** on capable hardware.
* **Interactive particle manipulation** — click & drag the nearest particle.

---

## ⌨️ Controls

* **`Insert`** — Toggle ImGui menu
* **`Space`** — Clear all particles
* **`F1`** — Freeze / stop all particles
* **Left mouse (hold)** — Move the nearest particle to the mouse and drag it
* Resize window — particles adapt to borders

---

## 🧩 ImGui Menu (items shown in screenshot)

**Top info**

* `FPS: <value>`
* `Particles: <value>`

**GRAVITY**

* `Gravity X` (slider)
* `Gravity Y` (slider)

**ELASTICITY**

* `Elasticity` (slider)

**General toggles**

* `Turbo Mode` (checkbox)
* `Show Grid` (checkbox)
* `Show Ball Grid Index` (checkbox)

**FIGURES** (spawnable)

* `Square` (checkbox)
* `Triangle` (checkbox)
* `Circle` (checkbox)
* `Ball` (checkbox)
* `Static Ball` (checkbox)
* `Stick` (checkbox)
* `Chain` (checkbox)

**Shape / object parameters (sliders)**

* `Square size`
* `Triangle size`
* `Circle size`
* `Circle segments`
* `Ball size`
* `Static Ball size`
* `Chain distance`

---

## ⚡ Performance

* Approx **10,000 particles** at **~60 FPS** on suitable hardware (CPU/GPU dependent).
* Spatial grid drastically reduces pairwise collision checks.
* Grid partitioning enables parallel processing of collision and resolution steps.

---

## ⚠️ Known Issues & Notes

* Occasionally particles may **go crazy** (sudden very high velocities). **Press `Space`** to clear all particles and resolve the issue.
* **Grid cell size = 5.** If you spawn balls larger than size 5, collision detection behaves poorly and instability becomes more frequent.

---

## ▶️ Demo / Showcase

**YouTube (placeholder):** `https://youtu.be/bxhSQGvEt70`

---

## 🛠️ Build / Run (suggested)

1. Build with a Windows C++ toolchain (Visual Studio recommended for DirectX11).
2. Link Dear ImGui with the DirectX11 backend.
3. Run the executable and press `Insert` to open the ImGui panel.


