# HLSL Experiments

# Experiment #1

For my first experiment I tried creating a mathematical alpha mask via HLSL code.

$\ radius =  length(pos-(frac(uv*gridsize))) $

$\ radiusLimit =  clamp(radius, 0.1, 0.5)$

I later created a material and input the $\ sin(TimeInputData) $ as the radius input.

![AlphaMask Circle](https://github.com/AryamaanPB/Unreal-Engine-Plugin-Experiments/blob/master/Content/GIF/AlphaMask.gif)

Shader complexity

![AlphaMask Circle Shader Complexity](https://github.com/AryamaanPB/Unreal-Engine-Plugin-Experiments/blob/master/Content/Inages/AlphaMaskShaderComplexity.png)