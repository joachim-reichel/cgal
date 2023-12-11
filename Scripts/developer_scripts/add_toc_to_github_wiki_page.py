import codecs
import re
import argparse
import sys
from urllib.parse import quote

parser = argparse.ArgumentParser()
parser.add_argument("filename", help="the Markdown file to process")
parser.add_argument(
    "--codebase",
    help="for a Markdown file of Codebase instead of Github",
    action="store_true",
)
parser.add_argument("--h1", help="support level one sections (h1)", action="store_true")
parser.add_argument("--max-level", help="maximum level of sections", type=int, default = 5)
args = parser.parse_args()


# a probably incomplete version to generate an anchor from a section name
def get_anchor(s):
    s = s.replace("`", "")
    s = s.replace("(", "")
    s = s.replace(")", "")
    s = s.replace(".", "")
    s = s.replace("#", "")
    s = s.replace(":", "")
    s = s.replace(",", "")
    s = s.replace(";", "")
    if args.codebase:
        s = s.replace("/", "-")
    else:
        s = s.replace("/", "")
    s = s.replace("<", "")
    s = s.replace(">", "")
    s = s.replace("+", "")
    s = s.replace("=", "")
    s = s.replace("?", "")
    s = s.replace("@", "")
    s = s.lstrip(" ")
    s = s.rstrip("\n")
    s = s.rstrip(" ")
    s = re.sub(r"\s+", "-", s)
    if not args.codebase:
        s = s.lower()
    if args.codebase:
        s = s.replace("'", "-and-39-")
    return "#" + quote(s)


# indices the nesting level (first level allowed is ##)
def get_level(s):
    m = re.search(r"^(#+)\s", s)
    if m:
        return len(m.group(1))
    else:
        return 0


def get_name(s):
    m = re.search(r"^#+\s+(.*)\s*$", s)
    if m:
        return m.group(1)
    else:
        return "ERROR: Section name extraction"


# generate the entry for one section
def get_toc_entry(s):
    name = get_name(s)
    if args.h1:
        level = get_level(s) - 1
    else:
        level = get_level(s) - 2
    anchor = get_anchor(s)

    if level < 0:
        return "ERROR: h1 sections are not allowed"

    res = "* [" + name + "](" + anchor + ")"
    for _ in range(0, level):
        res = "  " + res
    return res


# now the main
def main():
    filename = args.filename

    f = codecs.open(filename, "r", encoding="utf-8")

    if not f:
        print("Cannot open " + input + "\n")
        sys.exit()

    # look for <!--TOC--> the begin of the file
    line = f.readline()
    if line.find("<!--TOC-->") == -1:
        sys.exit()

    # skip current TOC
    line = f.readline()
    while line and line.find("<!--TOC-->") == -1:
        line = f.readline()

    if not line:
        sys.exit()

    buffer = ""
    toc = "<!--TOC-->\n\n# Table of Contents\n"

    verbatim_mode = False  # to ignore verbatim mode while looking for sections
    toc_empty = True
    for line in f.readlines():
        buffer += line
        if verbatim_mode:
            if line[:3] == "```":
                verbatim_mode = False
        else:
            if line[:3] == "```":
                verbatim_mode = True
            else:
                if line[0] == "#" and get_level(line) <= args.max_level:
                    toc += get_toc_entry(line) + "\n"
                    toc_empty = False
    toc += "\n<!--TOC-->\n"

    if not toc_empty:
        f.close()
        f = codecs.open(filename, "w", encoding="utf-8")
        f.write(toc)
        f.write(buffer)

if __name__ == "__main__":
    main()
