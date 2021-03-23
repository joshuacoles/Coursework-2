use array2d::Array2D;
use rand::Rng;
use std::ops::Range;
use std::collections::HashSet;
use rand::distributions::Uniform;
use std::fmt;

struct Grid {
    grid: Array2D<bool>
}

// To achieve uniform distribution of grids
fn random_indexes_in_range(range: Range<usize>, expected: usize) -> HashSet<usize> {
    let mut out = HashSet::with_capacity(expected);
    let uniform = Uniform::from(range);
    let mut rng = rand::thread_rng();

    while out.len() < expected {
        let i = rng.sample(uniform);
        out.insert(i);
    }

    return out;
}

impl Grid {
    fn new(lx: usize, ly: usize, n: usize) -> Grid {
        let indexes = random_indexes_in_range(0..lx * ly, n);
        let mut counter: usize = 0;

        let gen = || {
            counter += 1;
            return indexes.contains(&(counter - 1));
        };

        Grid {
            grid: Array2D::filled_by_row_major(gen, ly, lx)
        }
    }
}

impl fmt::Display for Grid {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        for row in self.grid.rows_iter() {
            for cell in row {
                write!(f, "{}", if *cell { '+' } else { '.' })?;
            }

            write!(f, "\n")?;
        }

        Ok(())
    }
}

pub fn main() {
    let lx = 6;
    let ly = 4;
    let n = 10;

    let g1 = Grid::new(lx, ly, n);
    let g2 = Grid::new(lx, ly, n);
    let g3 = Grid::new(lx, ly, n);
    println!("{}", g1);
    println!("{}", g2);
    println!("{}", g3);
}
