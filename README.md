# Static HTTP Input
A little project to demonstrate reading input with an nginx instance, without any scripting language installed!

How this works is that the program uses inotify, so it watches if the files have been opened. The client encodes their message and requests an entry to invoke a file read (or more), which inotify will pick up on.

I use a random get parameter to ensure that results are not cached. **Not sure if this works!** (future me note: it does!)

Annoyingly, the file must have at least a single byte in it.

Currently, the working example is in `./rainbowall` [sic]. You'll need to modify the directory in the C file that is being monitored.

# Why?

This was a little test for tilde.town, a fun little community hosted on a single box. We are not allowed to run web servers, so this is a way to accept input from a website simply by accessing statically hosted files (we get a directory of files we can host).

# Future:
    - Try to find unsuccessful accesses too! I want this to be some method to exfiltrate more data (e.g. access `/yes/now/BrowserFirefox54.11...blahblah`)
    - Alternatively, support recursive symlinks. This doesn't seem possible (at least on linux).
    - As its seemingly likely we can't encode arbitrary data well (due to lack of symlink recursion), I can multiplex requests.
        - This involves making a number of folders 0-100, and the client can send requests to them in parallel. The server will reorder them to work out the request.
        - obv we don't have to do 0-100, and we may want to do a bigger base to be more space conserving (0-9a-zA-Z- are good, as they're websafe)
