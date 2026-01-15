# XinkClock
### An xclock clone for e-ink displays

#### SDL Renderer
Includes an SDL renderer for more iterative development cycles.  

The SDL renderer for desktop requires sdl2 and sdl2_gfx.  
On macOS run the following.
```
brew install sdl2
brew install sdl2_gfx
```

To compile the SDL renderer on desktop
```
cd desktop
make
./xclock
```

<p align="center">
  <img src="sdl-xinkclock.png" alt="SDL XinkClock on macOS" width="800" height="694">
  <br>
  <em>SDL XinkClock on macOS</em>
</p>

