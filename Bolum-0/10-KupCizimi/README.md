![kupCizimi](resimler/kupCizimi.gif)

kodu duzelt?

<h2>Arka Yuz Eleme</h2>

Simdi kupu ciziyoruz iyi guzelde, bu kupun arkada kalan ucgenlerinide cizim sistemine yolluyoruz bosa dongu donuyor ve ilerde binlerce ucgene sahip modeller yukleyecegiz ornegin xyz_dragon.obj [modeller](https://github.com/alecjacobson/common-3d-test-models) ~800.000 ucgen. 

![arka yuz eleme karsilastirma](resimler/ayeKapali.png)

![arka yuz eleme acik](resimler/ayeAcik.png)

Evet yaklasik ~3-4 fps kurtarma operasyonuna baslayalim(bu kadar dusuk fps olmasinin sebebi tek cekirdek uzerinde donuyor program)


![aye_4](resimler/aye4.png)

- Karenin bir ucgenini alip kenara cekelim kose noktalari ABC olsun
```cpp
    Vector3 vectorA = transformedPoints[0];
    Vector3 vectorB = transformedPoints[1];
    Vector3 vectorC = transformedPoints[2];
```


![aye_3](resimler/aye3.png)

- A noktasindan B noktasina giden vektoru B-A yi hesaplayalim
- Ayni sey C-A icinde yapalim

```cpp
    Vector3 vectorAB = vectorB - vectorA;
    Vector3 vectorAC = vectorC - vectorA;
```      

![aye_2](resimler/aye2.png)
- Ucgene dik(normal) vektoru capraz carpim ile buluyoruz 
- Uzunluguna ihtiyacimiz olmadigi icin birim vektor hale getiriyoruz
  
  Elimizdeki bu normal vektor ucgenin neresinin DIS YUZEY oldugunu bildirmekte

```cpp
    Vector3 normal = vectorAB.cross(vectorAC);
    normal.normalize();
```
        
![aye_1](resimler/aye1.png)

```cpp
    Vector3 cameraRay = camera.position - vectorA;
```

![aye_0](resimler/aye0.png)

```cpp
    dotNormalCamera = normal.dot(cameraRay);
    if (cullmode == CullMode::ACTIVE)
    {
        if (dotNormalCamera <= 1)
        {
            continue;
        }
    }
```
<h2> Sonuc </h2>

![arkayuzEleme](resimler/arkaYuzEleme.gif)
