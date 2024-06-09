# rubus-gui

- Rendering: Skia
- Layout: Flexbox algorithm

## Features

- display mode
    - shown
    - hidden
    - collapsed
- rounded rect
- text
- image
- overflow clipping
- overflow scrolling
- sizing
    - self
    - parent
    - fit children
- layout
    - margin
    - padding
    - flexbox
        - wrap
        - align
            - start
            - center
            - end
- mouse event
    - enter / leave
    - over / out
    - down / up
    - click

## TODO

- feat: screen scaling
- feat: z-index / layer
- feat: border
- feat: shadow
- feat: min-max size
- feat: flex basis, shrink, grow
- feat: flex align
    - space-between
    - space-around
    - space-evenly
    - gap
- feat: image
    - repeat
    - nine patch
- feat: input
    - keyboard
- feat: focus
- optimize: layout caching
- optimize: incremental layout
- bug: resizing when scrolled

## Reference

- z-index:
    - <https://en.wikipedia.org/wiki/Timsort>
- border-radius:
    - <https://www.codingfactory.net/10957>
- flexbox:
    - <https://apost.dev/863/>
    - <https://studiomeal.com/archives/197>
    - <https://www.youtube.com/watch?v=Ns12ALe8aqI>
    - <https://stackoverflow.com/questions/34352140/what-are-the-differences-between-flex-basis-and-width>
- skia paragraph ignoring space:
    - <https://groups.google.com/g/skia-discuss/c/WXUVWrcgiko>
