<h1 align="center">
<a href="https://alien-project.org" target="_blank">ALiEn - Explore the world of artificial life</a>
</h1>

<h1 align="center">
<img src="img/alien.png" width=100%>
</h1>

alien is a GPU-accelerated physics engine with focus on artificial life computations.
Each simulated body has an internal structure which is modeled by a graph and can perform both physical actions and information processing by circulating tokens along the nodes.
The engine comes with a world builder, which makes it easy to construct universes according to your own ideas.

**Main features**
- realistic physical calculations of heat dissipation, collisions, bondings, damages, rotational forces, etc.
- programmable matter approach for simulating digital organisms and evolution
- built-in graph editor for designing own machines 
- simulation and rendering on GPU

The simulation code is written entirely in CUDA and highly optimized for large-scale real-time simulations of millions of bodies and particles.
Development is driven by the desire to simulate entire artificial ecosystems in order to imitate the conditions of (pre-)biotic evolution and thus better understand their mechanisms.

<img src="img/engine.gif" width=100%>

**Further information and artworks**
* [Website](https://alien-project.org)
* [YouTube](https://youtube.com/channel/UCtotfE3yvG0wwAZ4bDfPGYw)

## Documentation
Please visit [alien-project.org](https://alien-project.org/documentation.html) for a comprehensive documentation of the program and the underlying model.

## Installer
For Windows 10, an installer is available that provides 64-bit binaries of alien. [Download](https://alien-project.org/downloads.html).

## How to build the sources
To build alien you need Microsoft Visual Studio 2015 or higher. You find the solution file in msvc/Gui/alien.sln.
The following third-party libaries are necessary and should be installed:
- Qt 5.8 or higher
- boost library [version 1.65.1](https://www.boost.org/users/history/version_1_65_1.html) needs to be installed in external/boost_1_65_1 of the repository
- CUDA 9.0
gtest framework is already contained in the repository in external/gtest.

## License
alien is licensed under the [GPLv3](LICENSE).
