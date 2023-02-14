use rand::RngCore;
use std::{collections::{HashSet, HashMap}, thread::JoinHandle, sync::Arc};
use std::{
    sync::mpsc::{channel, Receiver, Sender}
};
use data_encoding::HEXLOWER;
use std::sync::Mutex;

#[allow(dead_code)]

fn main() {
    let part_bits: u8 = 1; // does it make sense if this could be 0?
    let part_mask: u16 = 1 << (part_bits - 1); // paper uses up to 18 bits, but 16 is super convenient
    let data = gen_data(usize::pow(2, 16));
    println!("{}", sha256::digest(&data[0].key.to_ne_bytes()));
    let hash = sha256::digest(&data[0].key.to_ne_bytes());
    let hash_bytes = HEXLOWER.decode(hash.as_bytes()).unwrap();
    let datastore = Arc::new(Mutex::new(ConcurrentOutputDataStore::new(10)));
    concurrent_output_worker(data, part_mask, datastore);
    
    // let foo = Box::new(ConcurrentOutputDataStore { data: Vec::with_capacity(0), mutex: Mutex::new(0) }) as Box<dyn DataStore + Send>;
    // let (tx, rx): (Sender<Box<dyn Bar + Send>>, Receiver<Box<dyn Bar + Send>>) = channel();

    // let ds = Box::new(ConcurrentOutputDataStore::new(0)) as Box<dyn DataStore + Send>;

    println!("{hash_bytes:?}")
}

fn get_partition(bytes: u16, mask: u16) -> u16 {
    return bytes & mask;
}

fn get_hash_from_u64(uint: u64) -> [u8; 32] {
    return get_hash(&uint.to_ne_bytes());
}

fn get_hash(bytes: &[u8]) -> [u8; 32] {
    let hash_string = sha256::digest(bytes);
    let hash_bytes = HEXLOWER.decode(hash_string.as_bytes()).unwrap();
    return hash_bytes.try_into().unwrap();
}

fn concurrent_output_worker(tuples: Vec<DataTuple>, partition_mask: u16, data_store: Arc<Mutex<impl DataStore + 'static>>) -> JoinHandle<i32> {
    return std::thread::spawn(move || {
        tuples.iter().for_each(|tuple| {
            let hash = get_hash_from_u64(tuple.key);
            if let Ok(mut x) = data_store.lock() {
                x.store(&hash, &tuple);
                // YOYO
            }
        });
        return 0;
    });
}

// fn independent_output_worker(
//     tuples: Vec<DataTuple>,
//     partition_mask: u16,
// ) -> JoinHandle<HashMap<u16, Vec<DataTuple>>> {
//     let partition_count = partition_mask * 2; 
//     return std::thread::spawn(move || {
//         let mut map = HashMap::new();
//         let hash = sha256::digest(&tuples[0].key.to_ne_bytes());
//         let partition_by = &hash.as_bytes()[0..2];
//         let key: u16 = u16::from(partition_by[0]) + u16::from(partition_by[1]);
//         if(map.contains_key(&key)) {
//             let list = map.get(&key).expect(&format!("Map does not contain key {}", key));
//             *list.insert(index, element)
//         }
//         // map.insert(key, v)
//         return map;
//     });
// }


trait DataStore : Send + Sync {
    fn new(size: usize) -> Self;
    fn store(self, hash: &[u8], tuple: &DataTuple);
}

struct ConcurrentOutputDataStore {
    data: Vec<DataTuple>,
    index: usize
}


impl DataStore for ConcurrentOutputDataStore {
    fn new(size: usize) -> Self {
        return Self {
            data: Vec::with_capacity(size),
            index: 0,
        }
    }

    fn store(mut self, _hash: &[u8], tuple: &DataTuple) {
        self.data.insert(self.index, *tuple);
        self.index += 1
    }

    

}

fn gen_data(count: usize) -> Vec<DataTuple> {
    let mut rng = rand::thread_rng();
    let mut datas = Vec::with_capacity(count);
    let mut used_keys = HashSet::<u64>::new();
    for _i in 0..count {
        let mut k = rng.next_u64();
        while used_keys.contains(&k) {
            println!("{} is already used", k);
            k = rng.next_u64();
        }
        used_keys.insert(k);
        let p = rng.next_u64();
        datas.push(DataTuple { key: k, payload: p });
    }
    datas
}

struct DataTuple {
    key: u64,
    payload: u64,
}
