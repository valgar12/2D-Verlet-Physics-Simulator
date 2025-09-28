2D Verlet Physics Simulator

A 2-month project — a 2D particle-based physics simulator implemented with Verlet Integration to learn physics engines and improve C++ skills. Renders with Dear ImGui + DirectX11 and includes a full ImGui debug/control menu.

Description

This simulator uses Verlet integration for stable constraint solving, supports multiple spawnable shapes, and provides a live ImGui panel (open with Insert) for tuning physics and visuals. Collisions are detected using a spatial grid and the grid is partitioned to enable multithreading.

Features

Verlet Integration for particle constraints and stability.

Spatial grid for collision detection (only checks collisions when 2+ balls share the same grid cell).

Grid partitioning for multithreaded collision/resolution work.

Live ImGui menu for runtime tuning and debugging.

Window-resizable — particles adapt to new window borders.

Supports around ~10k particles at ~60 FPS on suitable hardware.

Controls

Insert — Toggle ImGui menu

Space — Clear all particles

F1 — Freeze / stop all particles

Left mouse (hold) — Move the nearest particle to the mouse and drag it

Resize window — particles adapt to borders

ImGui Menu (items shown in the screenshot)

Top info

FPS: <value>

Particles: <value>

GRAVITY

Gravity X (slider)

Gravity Y (slider)

ELASTICITY

Elasticity (slider)

General toggles

Turbo Mode (checkbox)

Show Grid (checkbox)

Show Ball Grid Index (checkbox)

FIGURES (spawnable)

Square (checkbox)

Triangle (checkbox)

Circle (checkbox)

Ball (checkbox)

Static Ball (checkbox)

Stick (checkbox)

Chain (checkbox)

Shape / object parameters (sliders)

Square size

Triangle size

Circle size

Circle segments

Ball size

Static Ball size

Chain distance

Performance

Approx 10,000 particles at ~60 FPS on capable hardware (depends on CPU/GPU).

Collision checks are limited by the spatial grid which reduces pairwise checks dramatically.

Grid partitioning allows parallel processing of collision/resolution.

Known Issues & Notes

Occasionally particles may “go crazy” (very high velocities). Press Space to clear and resolve the issue.

Grid cell size = 5. Spawning balls larger than size 5 causes poor collision behavior and increases the chance of instability.

The YouTube showcase link below is a placeholder — replace with your uploaded video URL.

Demo / Showcase

YouTube (placeholder): https://youtu.be/EXAMPLE_SHOWCASE
