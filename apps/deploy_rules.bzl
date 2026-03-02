"""
deploy_rules.bzl
"""

load("@rules_bundle//:defs.bzl", "bundle_tar")
load("@rules_oci//oci:defs.bzl", "oci_image", "oci_load")
load("//libs/bazel:utils.bzl", "executable_path")

def create_deploy(name, target):
    """Bundles a cc_binary into a deployable OCI container image.

    Args:
        name: Name of the oci_image target.
        target: Label of the cc_binary to package, e.g. "//apps:simple_game".

    Creates:
        name_bundle.tar.gz
        name_image
        name_loader
    """

    executable_name = "{}_exe".format(name)

    executable_path(
        name = executable_name,
        binary = target,
    )

    bundle_tar(
        name = "{}_bundle".format(name),
        compression = "gz",
        executables = {
            "": target,
        },
        output = "{}_bundle.tar.gz".format(name),
    )

    oci_image(
        name = "{}_image".format(name),
        base = "@debian_slim",
        entrypoint = executable_name,
        tars = [
            "{}_bundle".format(name),
        ],
    )

    oci_load(
        name = "{}_loader".format(name),
        image = "{}_image".format(name),
        repo_tags = ["{}:latest".format(name)],
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
