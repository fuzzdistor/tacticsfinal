project(imgui)

add_library(imgui
        imgui.cpp
        imgui-SFML.cpp
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_tables.cpp
        imgui_widgets.cpp
        imconfig.h
        imgui.h
        imgui-SFML.h
        imgui-SFML_export.h
        imgui_internal.h
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
        )
if (MSVC)
  target_link_libraries(imgui sfml-graphics sfml-window sfml-audio sfml-system)
else ()
  target_link_libraries(imgui sfml-graphics sfml-window sfml-audio sfml-system GL)
endif ()