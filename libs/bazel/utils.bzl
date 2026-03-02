"""Utility rules for bazel"""

ExecutablePathInfo = provider("Returns the path to the executable and the launcher script", fields = ["path", "launcher"])

def _executable_path_impl(ctx):
    exe = ctx.executable.binary
    bundle_path = ctx.workspace_name + "/" + exe.short_path

    # This file is used by oci_image (one path per line)
    # AND as the executable for bazel run (via DefaultInfo.executable)
    launcher = ctx.actions.declare_file(ctx.label.name + ".sh")
    ctx.actions.write(
        output = launcher,
        content = "#!/bin/sh\nexec \"{path}\" \"$@\"\n".format(path = bundle_path),
        is_executable = True,
    )

    entrypoint = ctx.actions.declare_file(ctx.label.name)
    ctx.actions.write(
        output = entrypoint,
        content = bundle_path,
    )

    return [
        DefaultInfo(
            executable = launcher,  # bazel run uses this
            files = depset([entrypoint]),  # oci_image uses this
            runfiles = ctx.runfiles(files = [exe, launcher]),
        ),
    ]

executable_path = rule(
    implementation = _executable_path_impl,
    executable = True,  # makes the rule itself runnable with `bazel run`
    attrs = {
        "binary": attr.label(
            executable = True,
            cfg = "target",
            mandatory = True,
        ),
    },
)
