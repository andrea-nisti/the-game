load("@rules_appimage//appimage:appimage.bzl", "appimage")
load("@rules_oci//oci:defs.bzl", "oci_image", "oci_load")
load("@tar.bzl", "mtree_mutate", "mtree_spec", "tar")

# def _get_package_from_target(target):
#     sender_base_image_name = target.label.package
#     return sender_base_image_name

def data_layer(name, target, visibility, strip_prefix = "", include_runfiles = False):
    mtree_spec(
        name = "{}_mtree".format(name),
        srcs = [target],
        include_runfiles = include_runfiles,
    )

    mtree_mutate(
        name = "{}_mutate".format(name),
        mtree = ":{}_mtree".format(name),
        strip_prefix = strip_prefix,
    )

    tar(
        name = "{}".format(name),
        srcs = [target],
        mtree = ":{}_mutate".format(name),
        visibility = visibility,
    )

def create_deploy(name, target, visibility, strip_prefix = "", include_runfiles = False):
    executable_name = "{}_appimage".format(name)
    appimage(
        name = executable_name,
        binary = target,
    )

    data_layer(
        name = "_{}_dl".format(name),
        include_runfiles = include_runfiles,
        target = ":{}_appimage".format(name),
        visibility = ["//visibility:private"],
    )

    oci_image(
        name = "{}_image".format(name),
        base = "@debian_slim",
        entrypoint = [
            "./apps/{}".format(executable_name),  # TODO: extract path target
            "--appimage-extract-and-run",
        ],
        tars = [
            ":_{}_dl".format(name),
        ],
    )

    oci_load(
        name = "{}_loader".format(name),
        image = "{}_image".format(name),
        repo_tags = ["{}_image:latest".format(name)],
    )

    # expand_template_rule(
    #     name = "{}_compose".format(name),
    #     template = "//deploy/core:docker-compose.yml.in",
    #     substitutions = {
    #         "%SENDER_IMAGE%": "{}_image".format(name),
    #         "%SENDER_IPV4%": sender_ipv4,
    #     },
    #     is_executable = False,
    #     out = "compose_{}.yml".format(name),
    # )

    # expand_template_rule(
    #     name = "{}_run".format(name),
    #     template = "//deploy/core:run.sh.in",
    #     substitutions = {
    #         "%COMPOSE_FILE%": "$(rootpath :{}_compose)".format(name),
    #         "%SENDER_LOADER%": "{}_loader".format(name),
    #     },
    #     is_executable = True,
    #     data = ["{}_compose".format(name), "{}_loader".format(name)],
    #     out = "{}_run.sh".format(name),
    # )

    # sh_binary(
    #     name = name,
    #     srcs = ["{}_run".format(name)],
    #     data = ["{}_compose".format(name)],
    # )
