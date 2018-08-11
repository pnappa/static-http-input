# Static HTTP Input
A little project to demonstrate reading input with an nginx instance, without any scripting language installed!

How this works is that the program uses inotify, so it watches if the files have been opened. The client encodes their message and requests an entry to invoke a file read (or more), which inotify will pick up on.

I use a random get parameter to ensure that results are not cached. **Not sure if this works!** (future me note: it does!)

Annoyingly, the file must have at least a single byte in it.

# Future:
Try to find unsuccessful accesses too! I want this to be some method to exfiltrate more data (e.g. access `/yes/now/BrowserFirefox54.11...blahblah`)
