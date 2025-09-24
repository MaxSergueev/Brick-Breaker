# Predator-Prey Boid Simulation

A dynamic flocking simulation featuring three competing species in a predator-prey relationship, built with C++ and Raylib.
![Boids](https://github.com/user-attachments/assets/2f6ef0ee-48b7-427a-84be-58cc69b2fa47)

## Overview

This simulation implements the classic Boid algorithm with additional predator-prey behaviors and environmental interactions. The simulation runs with 150 autonomous agents divided into three distinct groups, each with unique behaviors and characteristics.

## Core Mechanics

### Basic Flocking Behaviors
- **Separation:** Avoid crowding nearby flockmates
- **Alignment:** Match velocity with nearby flockmates
- **Cohesion:** Move toward average position of flockmates

### Advanced Behaviors
- **Predator-Prey Relationships:** Rock-paper-scissors style hunting pattern
- **Obstacle Avoidance:** Agents avoid obstacles of different colors
- **Safe Zones:** Agents can enter anemone structures of matching colors

### Agent Types
1. **Purple Boids**
   - Highly avoidant behavior
   - Fastest maximum speed
   - Quick turning capability

2. **Tan Boids**
   - Aggressive hunters
   - Strong pack behavior
   - Good pursuit speed

3. **Green Boids**
   - Balanced behavior
   - Moderate speed and turn rate
   - Equal hunting and fleeing capabilities

## Technical Details
- **Agent Count:** 150 boids (50 of each type) though can be expanded
- **Frame Rate:** 60 FPS
- **Screen Size:** 1280x720
- **Built with:** C++14, Raylib

## Predator-Prey Cycle
Purple → Green → Tan → Purple (each species hunts the next in the cycle)
