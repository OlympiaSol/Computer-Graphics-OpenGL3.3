
# OpenGL Fireballs Simulation

## Introduction

This project is an immersive simulation developed using OpenGL 3.3 (and later versions) to practice basic libraries for 2D and 3D graphics. The application simulates fireballs falling from the sky and impacting the ground, creating a dynamic and visually engaging scene.

## Features

- **Dynamic Fireball Simulation**: Fireballs appear randomly in the sky and descend towards the ground, simulating impacts with explosive visual effects.
- **Interactive Camera Controls**: The camera supports movement along global coordinate axes, including rotations around the X and Z axes, and zoom in/out functionality for enhanced observation.
- **Texture Application**: Both the ground and fireballs are textured, with the ground represented as a grid of 400 squares and fireballs using a fiery texture to enhance realism.
- **Customizable Settings**: Users can interact with the simulation through keyboard inputs, including generating new fireballs and adjusting the camera perspective.

## Technical Details

- **Window Setup**: A basic window of 1000x1000 pixels with a black background and customizable title.
- **Ground Representation**: A grid on the XY plane at Z=0, textured with selectable images to simulate the ground.
- **Fireballs**: Spheres loaded from `ball.obj` files, textured with `fire.jpg`, and programmed to disappear upon impact, leaving a "crater" texture on the ground.

## Bonus Features

- **Sound and Lighting Effects**: Enhance the explosion effect with sound and lighting upon fireball impact.
- **Speed Adjustment**: Use keyboard inputs to increase or decrease the speed of the fireball's descent.
- **Enhanced Ground Damage**: Implement logic to create larger damage on the ground if a fireball hits an existing crater.
- **Dynamic Ground Geometry**: Modify the ground's geometry to reflect the impact of falling fireballs.

## Getting Started

- Compile and run the provided `Source-1C.cpp` program, utilizing GLFW, GLEW, and GLM libraries for graphics rendering.
- Follow the in-application prompts to interact with the simulation.

### Project Overview

The OpenGL Fireballs Simulation project offers a unique opportunity to delve into graphics programming, focusing on dynamic object simulation and interactive camera controls. Through this project, participants will gain hands-on experience with OpenGL, enhancing their understanding of 3D graphics and real-time simulation techniques.
