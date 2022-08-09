add_library (debris-renderer STATIC "dx11.cpp")
target_link_libraries(debris-renderer d3d11 dxgi dxguid d3dcompiler winmm d2d1 dwrite)
