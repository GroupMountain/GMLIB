add_rules("mode.debug", "mode.release", "mode.releasedbg")

add_repositories("liteldev-repo https://github.com/LiteLDev/xmake-repo.git")

if not has_config("vs_runtime") then
    set_runtimes("MD")
end

add_requires("levilamina") -- or add_requires("levilamina x.x.x") to specify target LeviLamina version

target("GMLIB") -- Change this to your plugin name.
    add_cxflags(
        "/EHa",
        "/utf-8"
    )
    add_files(
        "src/**.cc"
    )
    add_defines(
        "GMLIB_EXPORTS"
    )
    add_includedirs(
        "src",
        "include"
    )
    add_packages(
        "levilamina"
    )
    add_shflags(
        "/DELAYLOAD:bedrock_server.dll"
    )
    set_exceptions("none")
    set_kind("shared")
    set_languages("cxx23")

    after_build(function (target)
        local plugin_packer = import("scripts.after_build")

        local plugin_define = {
            pluginName = target:name(),
            pluginFile = path.filename(target:targetfile()),
        }
        
        plugin_packer.pack_plugin(target,plugin_define)
    end)
