Program ana donguye girdiginden beri gecen sure

```cpp
uti::Timer timer;
while(f_running)
{
    std::cout << timer.getElapsedTime_seconds() <<"\n";    
    
    ...
}
```


kareler arası geçen süre
$$
\Delta t
$$

```cpp
uti::Timer timer;
while(f_running)
{
    std::cout << timer.getElapsedTime_seconds() <<"\n";    
    timer.start();

    ...
}
```

FPS saniye basina kare sayisi

```cpp
uti::Timer timer;
while(f_running)
{
    float dt = timer.getElapsedTime_seconds();
    float fps = 1.0f / dt;
    std::cout << fps << "\n";
    timer.start();

    ...
}
```
