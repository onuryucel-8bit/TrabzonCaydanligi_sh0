```cpp
    void update()
    {
        for(size_t i = 0; i < meshFaces.size(); i++)
        {
            ...

            Vector3 vectorA = transformedPoints[0];
            Vector3 vectorB = transformedPoints[1];
            Vector3 vectorC = transformedPoints[2];

            ...
            ...

            //---Derinlik---/
            if (appState.m_depthTest == DepthTest::PAINTER_AVERAGE)
            {
                projectedTrig.depthTestValue = (vectorA.z + vectorB.z + vectorC.z) / 3;
            }
            else if (appState.m_depthTest == DepthTest::PAINTER_DISTANCE)
            {
                Vector3 trigCenter;
                trigCenter.x = (vectorA.x + vectorB.x + vectorC.x) / 3;
                trigCenter.y = (vectorA.y + vectorB.y + vectorC.y) / 3;
                trigCenter.z = (vectorA.z + vectorB.z + vectorC.z) / 3;

                Vector3 distance;

                distance = trigCenter - appState.m_camera.position;

                projectedTrig.depthTestValue = distance.x * distance.x + distance.y * distance.y + distance.z * distance.z;
            }

            renderTrigs.emplace_back(projectedTrig);
        

            if (appState.m_depthTest == DepthTest::PAINTER_AVERAGE ||
                appState.m_depthTest == DepthTest::PAINTER_DISTANCE)
            {
                std::sort(renderTrigs.begin(), renderTrigs.end(),
                    [](const Triangle& a, const Triangle& b)
                    {
                        return a.depthTestValue > b.depthTestValue;
                    }
                );
            }
        }//for(...)
    }//void update()
```

![derinlik1](resimler/derinlik1.png)
![derinlik2](resimler/derinlik2.png)
![derinlik3](resimler/derinlik3.png)


![derinlik](resimler/derinlik.gif)