Application Entities
====================

Banner
two_text lines




CartelMania Rendering Pipeline
==============================

1. Create two paths for the banner text strings.
2. Subdivide both paths with desired resolution.
3. 

Banner::Render()
{
    BuildPaths();
    SubdividePaths();
    ApplyEffects();
    ApplyShape();
    
}