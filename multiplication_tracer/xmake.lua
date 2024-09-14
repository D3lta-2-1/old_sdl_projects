add_rules("mode.debug","mode.release")
add_rules("plugin.vsxmake.autoupdate")
add_requires("libsdl")

target("multiplication_tracer")
    set_kind("binary")
    set_targetdir("bin")
    set_languages("CXX17")
    add_files("src/*.cpp")
    add_headerfiles("src/*.hpp")
    add_packages("libsdl")