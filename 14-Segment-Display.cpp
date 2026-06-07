/*
 * Q2 Part II: 14-Segment Display Pattern Generator
 *
 * Encodes characters using a 14-segment display bitmask and renders them
 * as ASCII art in a decorative border. Supports: A-Z, 0-9, / \ , - _
 *
 * Segment bit layout (LSB = bit 0):
 *   bit 0  = a  (top horizontal)
 *   bit 1  = b  (upper-right vertical)
 *   bit 2  = c  (lower-right vertical)
 *   bit 3  = d  (bottom horizontal)
 *   bit 4  = e  (lower-left vertical)
 *   bit 5  = f  (upper-left vertical)
 *   bit 6  = G1 (upper-left diagonal, top half)
 *   bit 7  = g2 (upper-right diagonal, top half)
 *   bit 8  = h  (upper-left diagonal)
 *   bit 9  = j  (top-center vertical, upper)
 *   bit 10 = k  (upper-right diagonal)
 *   bit 11 = l  (lower-left diagonal)
 *   bit 12 = m  (bottom-center vertical)
 *   bit 13 = n  (lower-right diagonal)
 */

#include <iostream>
using namespace std;

// Returns the 14-segment bitmask for a given character
int segmentMap(int ascii) {
    switch (ascii) {
        case 'A': case 'a': return 0b000000011110111;
        case 'B': case 'b': return 0b001001010001111;
        case 'C': case 'c': return 0b000000000111001;
        case 'D': case 'd': return 0b001001000001111;
        case 'E': case 'e': return 0b000000001111001;
        case 'F': case 'f': return 0b000000001110001;
        case 'G': case 'g': return 0b000000010111101;
        case 'H': case 'h': return 0b000000011110110;
        case 'I': case 'i': return 0b001001000001001;
        case 'J': case 'j': return 0b000000000011110;
        case 'K': case 'k': return 0b011010001110000;
        case 'L': case 'l': return 0b000000000111000;
        case 'M': case 'm': return 0b000010100110110;
        case 'N': case 'n': return 0b010000100110110;
        case 'O': case 'o': return 0b000000000111111;
        case 'P': case 'p': return 0b000000011110011;
        case 'Q': case 'q': return 0b010000000111111;
        case 'R': case 'r': return 0b010000011110011;
        case 'S': case 's': return 0b000000011101101;
        case 'T': case 't': return 0b001001000000001;
        case 'U': case 'u': return 0b000000000111110;
        case 'V': case 'v': return 0b000110000110000;
        case 'W': case 'w': return 0b010100000110110;
        case 'X': case 'x': return 0b010110100000000;
        case 'Y': case 'y': return 0b000000011101110;
        case 'Z': case 'z': return 0b000110000001001;
        case '0':           return 0b000110000111111;
        case '1':           return 0b000010000000110;
        case '2':           return 0b000000011011011;
        case '3':           return 0b000000010001111;
        case '4':           return 0b000000011100110;
        case '5':           return 0b010000001101001;
        case '6':           return 0b000000011111101;
        case '7':           return 0b000000000000111;
        case '8':           return 0b000000011111111;
        case '9':           return 0b000000011101111;
        case '/':           return 0b000110000000000;
        case '\\':          return 0b010000100000000;
        case '-':           return 0b000000011000000;
        case '_':           return 0b000000000001000;
        case ' ':           return 0b000000000000000;
        case ',':           return 0b000100001000000;
        default:            return 0b000000000000000;
    }
}

/*
 * Renders one row (scanline i) of a single character given its bitmask.
 * n      = total height of one character cell
 * parts  = n/10, used as thickness for segments
 */
void renderCharRow(int map, int i, int n, int parts) {
    for (int j = 0; j < n + 2; j++) {
        // Top decoration rows (border area above character)
        if (i < 0) {
            if (i == -3 || i == -6) cout << '*';
            else {
                if ((j % 2) ^ ((i + 5) % 2)) cout << '^';
                else cout << '.';
            }
            continue;
        }
        if (i == -2 || i == -1) { cout << ' '; continue; }

        // Segment 'a': top horizontal bar (row 0 to parts-1)
        if (i < parts && i >= 0 && j < n) {
            if (map & 1) cout << '#';                           // a
            else if ((map >> 1 & 1) && j >= n - parts && j < n) cout << '#'; // b
            else if ((map >> 5 & 1) && j < parts)              cout << '#'; // f
            else cout << ' ';
            continue;
        }

        // Upper vertical band: segments f (left), h/j/k (diagonals/center), b (right)
        if (i >= parts && i < 5 * parts) {
            if (j < parts) {
                cout << ((map >> 5 & 1) ? '#' : ' ');          // f
            } else if (j >= parts && j < (n / 2) - 1) {        // h diagonal
                if ((j == i || i == j + 1) && (map >> 8 & 1)) cout << '#';
                else if (i == (5 * parts - 1) && (map >> 6 & 1)) cout << '#'; // G1
                else cout << ' ';
            } else if (j == n / 2 || j == (n / 2) - 1) {       // j center
                if (map >> 9 & 1) cout << '#';
                else if (i == (5 * parts - 1) && (map >> 7 & 1)) cout << '#'; // g2
                else if (i == (5 * parts - 1) && (map >> 6 & 1)) cout << '#'; // G1
                else cout << ' ';
            } else if (j > n / 2 && j < n - parts) {           // k diagonal
                if ((j == n - i - 1 || j == n - i) && (map >> 10 & 1)) cout << '#';
                else if (i == (5 * parts - 1) && (map >> 7 & 1)) cout << '#'; // g2
                else cout << ' ';
            } else if (j >= n - parts && j < n) {
                cout << ((map >> 1 & 1) ? '#' : ' ');          // b
            }
            continue;
        }

        // Lower vertical band: segments e (left), l/m/n (diagonals/center), c (right)
        if (i >= 5 * parts && i < 9 * parts) {
            if (j < parts) {
                cout << ((map >> 4 & 1) ? '#' : ' ');          // e
            } else if (j >= parts && j < (n / 2) - 1) {        // l diagonal
                if ((j == n - i - 2 || j == n - i - 1) && (map >> 11 & 1)) cout << '#';
                else if (i == (5 * parts) && (map >> 6 & 1)) cout << '#'; // G1
                else cout << ' ';
            } else if (j == n / 2 || j == (n / 2) - 1) {       // m center
                if (map >> 12 & 1) cout << '#';
                else if (i == (5 * parts) && (map >> 7 & 1)) cout << '#'; // g2
                else if (i == (5 * parts) && (map >> 6 & 1)) cout << '#'; // G1
                else cout << ' ';
            } else if (j > n / 2 && j < n - parts) {           // n diagonal
                if ((j == i || j == i + 1) && (map >> 13 & 1)) cout << '#';
                else if (i == (5 * parts) && (map >> 7 & 1)) cout << '#'; // g2
                else cout << ' ';
            } else if (j >= n - parts && j < n) {
                cout << ((map >> 2 & 1) ? '#' : ' ');          // c
            }
            continue;
        }

        // Segment 'd': bottom horizontal bar
        if (i >= 9 * parts && j < n && i < n) {
            if (map >> 3 & 1) cout << '#';
            else if ((map >> 12 & 1) && (j == n / 2 || j == (n / 2) - 1)) cout << '#'; // m
            else if ((map >> 4 & 1) && j < parts) cout << '#';             // e
            else if ((map >> 2 & 1) && j >= n - parts && j < n) cout << '#'; // c
            else cout << ' ';
            continue;
        }

        // Two-space gap between characters
        if (j > n && i >= 0 && i < n) { cout << "  "; break; }

        // Bottom decoration rows
        if (i == n || i == n + 1) { cout << ' '; continue; }
        if (i > n + 1) {
            if (i == n + 2 || i == n + 5) cout << '*';
            else {
                if ((j % 2) ^ ((i) % 2)) cout << '^';
                else cout << '.';
            }
            continue;
        }
    }
}

int main() {
    int n = 20;           // Height of each character (fixed at 20 as per assignment)
    int parts = n / 10;   // Segment thickness

    char input[100];
    cout << "Enter a name (max 100 characters): ";
    cin.getline(input, 100);

    // Print each scan line across all characters
    for (int i = -6; i < n + 6; i++) {
        // Left border
        if (i == -6 || i == n + 5)      cout << "*****";
        else if (i == -5 || i == n + 4) cout << "*.^.^";
        else if (i == -4 || i == n + 3) cout << "*^.^.";
        else if (i == -3 || i == n + 2) cout << "*.***";
        else                             cout << "* *  ";

        // Render each character for this scanline
        for (int k = 0; input[k] != '\0'; k++) {
            int map = segmentMap(input[k]);
            renderCharRow(map, i, n, parts);
        }

        // Right border
        if (i == -6 || i == n + 5)      cout << "*****";
        else if (i == -5 || i == n + 4) cout << ".^.^*";
        else if (i == -4 || i == n + 3) cout << "^.^.*";
        else if (i == -3 || i == n + 2) cout << "***.*";
        else                             cout << "  * *";

        cout << endl;
    }

    return 0;
}
