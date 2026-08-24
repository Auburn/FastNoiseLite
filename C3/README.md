## Getting Started

Here's an example for creating a 128x128 array of OpenSimplex2 noise

```c
// Create and configure noise state
FnlState noise = fnl::createState();
noise.noise_type = OPENSIMPLEX2;

// Gather noise data
float* noise_data = malloc(128 * 128 * float::size);
int index = 0;

for (int y = 0; y < 128; y++)
{
    for (int x = 0; x < 128; x++) 
    {
        noise_data[index++] = fnl::getNoise2D(&noise, x, y);
    }
}

// Do something with this data...

// Free data later
free(noise_data);
```
