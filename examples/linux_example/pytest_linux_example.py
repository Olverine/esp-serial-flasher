import pytest
import pexpect


@pytest.mark.linux
def test_linux_example():
    with pexpect.spawn(
        "./examples/linux_example/build/linux_flasher",
        timeout=10,
        encoding="utf-8",
    ) as p:
        # Check initial connection
        p.expect("Connected to target")
        p.expect("Transmission rate changed")

        # Check bootloader programming
        p.expect("Loading bootloader")
        p.expect("Erasing flash")
        p.expect("Start programming")
        p.expect("Finished programming")
        p.expect("Flash verified")

        # Check partition table programming
        p.expect("Loading partition table")
        p.expect("Erasing flash")
        p.expect("Start programming")
        p.expect("Finished programming")
        p.expect("Flash verified")

        # Check app programming
        p.expect("Loading app")
        p.expect("Erasing flash")
        p.expect("Start programming")
        p.expect("Finished programming")
        p.expect("Flash verified")

        # Check target output
        p.expect("Hello world!")
