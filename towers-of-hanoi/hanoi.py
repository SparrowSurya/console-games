"""
Tower Of Hanoi
--------------

This is a fun terminal based game to solve the problem.
"""

# terminal colors and styles
TEXT_UNDERLINE        = '\u001b[4m'
TEXT_DOUBLE_UNDERLINE = '\u001b[21m'
TEXT_RESET            = '\u001b[0m'
TEXT_DIM              = '\u001b[2m'
TEXT_BLINK            = '\u001b[6m'
TEXT_INVERT           = '\u001b[7m'
TEXT_HIDE             = '\u001b[8m'  # color same as background; text can be copied!
TEXT_STRIKE           = '\u001b[9m'
COLORS = [
    '\u001b[97m',  # white
    '\u001b[91m',  # red 
    '\u001b[92m',  # green
    '\u001b[93m',  # yellow
    '\u001b[94m',  # cyan
    '\u001b[95m',  # magenta
    '\u001b[96m',  # slate
    '\u001b[90m',  # grey
]


# drawing specific symbols
PILLAR = '┃'
DISC = '▆'
BASE = '▒' # '▆'
PROMPT = '❯❯❯ '

# box chars
BOX_TOP_LEFT_CORNER = '┏'
BOX_TOP_RIGHT_CORNER = '┓'
BOX_BOTTOM_LEFT_CORNER = '┗'
BOX_BOTTOM_RIGHT_CORNER = '┛'
BOX_VERTICAL_EDGE = '┃'
BOX_HORIZONTAL_EDGE = '━'


# data
STACK1: list[int] = []
STACK2: list[int] = []
STACK3: list[int] = []

LIMIT = len(COLORS)
DISC_SIZE = 4
USER_MOVES = 0

QUIT = 'QUIT!'
MOVES = ['12', '13', '23', '21', '31', '32', QUIT]

RULES = (
    'Only one disk may be moved at a time.',
    'Each move consists of taking the upper disk from one of the stacks\n   and placing it on top of another stack or on an empty rod.',
    'No disk may be placed on top of a disk that is smaller than it.'
)

# game functions

def show_info():
    """Displays info about game."""
    global COLORS, MOVES, TEXT_UNDERLINE, TEXT_RESET, QUIT
    print('\n', TEXT_UNDERLINE, "How to play:", TEXT_RESET, sep='')
    print('\tValid Options: [', end='')
    print(COLORS[3], MOVES[0], COLORS[0], sep='', end='')
    for opt in MOVES[1:]:
        print(', ', COLORS[3], opt, COLORS[0], sep='', end='')
    
    print(']\n\tExample: \'12\' moves DISC from Tower-1 to Tower-2')
    print(f'\tUse \'{QUIT}\' to exit the game', end='\n\n')

def show_rules():
    """Displays the rule of the game."""
    global COLORS, RULES, TEXT_UNDERLINE, TEXT_RESET
    print('\n', TEXT_UNDERLINE, "Objective:", TEXT_RESET, sep='')
    print('\tMove the entire stack to the last rod.', end='\n\n')
    print(TEXT_UNDERLINE, "Rules:", TEXT_RESET, sep='')
    for i, rule in enumerate(RULES):
        print(COLORS[4], f'{i+1}. ', COLORS[0], rule, sep='')
    print()

def show_error(*msg: object, sep: str | None = " ", end: str | None = "\n") -> None:
    """Displays the error message."""
    global COLORS
    print(COLORS[1] + 'ERROR: ' + COLORS[0], *msg, sep=sep, end=end)

def restart(size: int | None = None) -> None:
    """Resets the game state."""
    global USER_MOVES, DISC_SIZE, STACK1, STACK2, STACK3
    DISC_SIZE = size if isinstance(size, int) else DISC_SIZE 
    USER_MOVES *= 0

    STACK1.clear()
    STACK2.clear()
    STACK3.clear()

    STACK1.extend(list(range(DISC_SIZE, 0, -1)))
    

def create_disk(size: int, width: int) -> str:
    """Creates a single Disc."""
    global DISC, COLORS
    disc = DISC*(2*size+1)
    disk_block = f"{disc:^{width}}"
    return COLORS[size] + disk_block + COLORS[0]

def create_tower(stack: list[int], width: int, height: int) -> list[str]:
    """Creates single hanoi tower of given height."""
    global TEXT_DIM, TEXT_RESET, PILLAR
    pillar = [f"{TEXT_DIM}{PILLAR:^{width}}{TEXT_RESET}"]*(height - len(stack))
    discs = [create_disk(size, width) for size in reversed(stack)]
    return pillar + discs 


def draw() -> None:
    """Draws the scene."""
    global DISC_SIZE, STACK1, STACK2, STACK3
    global BOX_BOTTOM_LEFT_CORNER, BOX_BOTTOM_RIGHT_CORNER, BOX_TOP_LEFT_CORNER, BOX_TOP_RIGHT_CORNER
    global BOX_VERTICAL_EDGE, BOX_HORIZONTAL_EDGE, TEXT_INVERT, TEXT_RESET

    tower_width = 2*DISC_SIZE+1
    tower_height = DISC_SIZE+1

    box_indent = ' '*2
    padding = ' '*4
    display_width = tower_width*3 + len(padding)*6

    tower1 = create_tower(STACK1, tower_width, tower_height)
    tower2 = create_tower(STACK2, tower_width, tower_height)
    tower3 = create_tower(STACK3, tower_width, tower_height)


    # header
    print(
        '\n',
        box_indent, 
        BOX_TOP_LEFT_CORNER, 
        BOX_HORIZONTAL_EDGE*display_width, 
        BOX_TOP_RIGHT_CORNER,
        sep=''
    )

    # heading
    print(
        box_indent,
        BOX_VERTICAL_EDGE,
        TEXT_INVERT,
        f'{"Tower of Hanoi":^{display_width}}',
        TEXT_RESET,
        BOX_VERTICAL_EDGE,
        sep='',
    )

    # spacing
    print(
        box_indent,
        BOX_VERTICAL_EDGE,
        ' '*display_width,
        BOX_VERTICAL_EDGE,
        sep='',
    )

    # towers
    for t1, t2, t3 in zip(tower1, tower2, tower3):
        print(
            box_indent,
            BOX_VERTICAL_EDGE,
            padding,
            padding,
            t1,
            padding,
            t2,
            padding,
            t3,
            padding,
            padding,
            BOX_VERTICAL_EDGE,
            sep='',
        )

    # base
    print(
        box_indent,
        BOX_VERTICAL_EDGE,
        padding,
        TEXT_DIM,
        BASE*(display_width - len(padding)*2),
        TEXT_RESET,
        padding,
        BOX_VERTICAL_EDGE,
        sep=''
    )

    # numbering
    print(
        box_indent,
        BOX_VERTICAL_EDGE,
        padding,
        padding,
        f'{"1":^{tower_width}}',
        padding,
        f'{"2":^{tower_width}}',
        padding,
        f'{"3":^{tower_width}}',
        padding,
        padding,
        BOX_VERTICAL_EDGE,
        sep=''
    )

    # footer
    print(
        box_indent, 
        BOX_BOTTOM_LEFT_CORNER, 
        BOX_HORIZONTAL_EDGE*display_width, 
        BOX_BOTTOM_RIGHT_CORNER,
        sep='',
        end='\n\n'
    )


def get_move() -> str:
    """Returns valid move from User."""
    global PROMPT
    tries = 0
    mov = input(PROMPT)
    while not valid_move(mov):
        tries += 1
        print("Invalid move!")
        if tries%5 == 0:
            show_info()
        mov = input(PROMPT)
    return mov

def empty(stack: list[int]) -> bool:
    """Returns if stack is empty."""
    return stack == []

def top(stack: list[int]) -> int:
    """Returns (always) top of the stack."""
    return stack[-1]

def valid_move(mov: str) -> bool:
    """Returns if `mov` is a valid disc move."""
    global STACK1, STACK2, STACK3, MOVES
    if mov == '12':
        return (not empty(STACK1)) and (empty(STACK2) or top(STACK1) < top(STACK2))
    elif mov == '21':
        return (not empty(STACK2)) and (empty(STACK1) or top(STACK2) < top(STACK1))
    elif mov == '13':
        return (not empty(STACK1)) and (empty(STACK3) or top(STACK1) < top(STACK3))
    elif mov == '31':
        return (not empty(STACK3)) and (empty(STACK1) or top(STACK3) < top(STACK1))
    elif mov == '23':
        return (not empty(STACK2)) and (empty(STACK3) or top(STACK2) < top(STACK3))
    elif mov == '32':
        return (not empty(STACK3)) and (empty(STACK2) or top(STACK3) < top(STACK2))
    return mov in MOVES

def move(mov: str) -> None:
    """Makes move."""
    global USER_MOVES, STACK1, STACK2, STACK3
    if mov == '12':
        STACK2.append(STACK1.pop())
    elif mov == '21':
        STACK1.append(STACK2.pop())
    elif mov == '13':
        STACK3.append(STACK1.pop())
    elif mov == '31':
        STACK1.append(STACK3.pop())
    elif mov == '23':
        STACK3.append(STACK2.pop())
    elif mov == '32':
        STACK2.append(STACK3.pop())
    elif mov == QUIT:
        exit_game()
    USER_MOVES += 1

def solved() -> bool:
    """Returns status of match finished"""
    global DISC_SIZE, STACK3
    return STACK3 == list(range(DISC_SIZE, 0, -1))

def play_match() -> None:
    """A single Match."""
    draw()
    while not solved():
        mov = get_move()
        move(mov)
        draw()

def exit_game() -> None:
    """Exits the game."""
    print("Exiting ...")
    exit(0)

def ask_no_of_discs() -> int:
    """Returns no of discs from User."""   
    global COLORS, DISC_SIZE, LIMIT 
    print('\nOptions: [', end='')
    print(COLORS[3], '1', COLORS[0], sep='', end='')
    for opt in range(2, len(COLORS)):
        print(', ', COLORS[3], opt, COLORS[0], sep='', end='')

    print(f"]\nDefault: {DISC_SIZE}")
    try: 
        size = int(input("Enter the number of discs: "))
        assert 1 <= size < LIMIT
    except: 
        show_error("Invalid number of discs!")
        print("Choosing Defaul size", DISC_SIZE)
        size = DISC_SIZE
    return size 

def ask_exit() -> bool:
    """Asks if User want to quit game!"""
    while 1:
        wants_to_exit = input("Quit [Y/n]? ")
        if wants_to_exit == 'Y':
            return True
        elif wants_to_exit == 'n':
            return False
        elif wants_to_exit == '':
            continue
        else:
            show_error('Invalid option!')
            print('Choose an appropriate option')

def show_status():
    """Shows match status."""
    global USER_MOVES, DISC_SIZE
    min_moves = 2**DISC_SIZE-1
    accuracy =  (1 - ((USER_MOVES - min_moves) / USER_MOVES)) * 100 
    print(
        "Status",
        f"\tMoves Played: {USER_MOVES}",
        f"\tAccuracy: {accuracy:.2f}",
        sep='\n',
        end='\n\n'
    )

def wait_read() -> None:
    """Let user read the info provided."""
    global TEXT_HIDE, TEXT_RESET
    input('Press Enter to continue ...' + TEXT_HIDE)
    print(TEXT_RESET, end='\r')

def main() -> None:
    """Main game."""
    print("==================== Tower-of-Hanoi ====================")
    show_rules()
    wait_read()
    show_info()
    wait_read()
    while 1:
        discs = ask_no_of_discs()
        restart(discs)
        play_match()
        print('Yay, You did it!', end='\n\n')
        show_status()
        if ask_exit() == True:
            break
    exit_game()


if __name__ == '__main__':
    main()