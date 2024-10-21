import hashlib
import json
import time
from web3 import Web3

#llenar lo que falta con los datos de tu cuenta
ganache_url = 'https://sepolia.infura.io/v3/1ece952fe1284236a950b41fa084af6c'
account_from = ""
account_to = "0xa7F3c68f32B2f5E6C1B11c26258Ff015631b2Cd1"
private_key = ""
w3 = Web3(Web3.HTTPProvider(ganache_url))
blockchain = []

def create_transaction(nonce, to, value, gas, gasPrice, pk):
    tx = {
        'nonce': nonce,
        'to': to,
        'value': w3.to_wei(value, 'ether'),
        'gas': gas,
        'gasPrice': w3.to_wei(gasPrice, 'gwei')
    }
    #firmar la transaccion
    signed_tx = _
    return signed_tx

def send_eth_transaction():
    res = w3.is_connected()
    if (res):
        #usar la cantidad de transacciones como nonce
        nonce = _
        #fijar los valores del value, gas y el gasPrice. Hint: usa la funcion w3.eth.fee_history para determinar un valor que vaya a ser aceptado por la red
        #CUIDADO con mandar transacciones con valores de gas y gasPrice muy bajos, ya que pueden ser rechazadas por la red
        #prueba enviando un value pequeños, como unos cuantos wei
        signed_tx = create_transaction(nonce, account_to, _, _, _, private_key)
        tx_hash = w3.eth.send_raw_transaction(signed_tx.rawTransaction)
        return w3.eth.get_transaction(tx_hash) 
    else:
        return None

# Converts hash to a number and checks if it is prime
def simplePow(blockHash):
    hashInt = int(_, 16)
    return _

# Checks if a number is prime
def isPrime(n):
    return _

# Checks if a number is even
def isEven(n):
    return _

# Calculates a valid block hash and updates block if necessary
# Returns the block with a valid hash
def generateHash(block):
    blockString = json.dumps(_, sort_keys=True).encode()
    curr_hash = hashlib.sha256(_).hexdigest()
    return _

# Creates a new block
# Block structure is as follows:
# {
#     'index': current_index,
#     'timestamp': current_timestamp,
#     'data': {
#         'to': to_address,
#         'from': from_address,
#         'amount': amount
#      },
#     'prevHash': previous_block_hash,
#     'hash': block_hash,
#     'ctr': pow_counter
# }
# Returns the new block
def createBlock(prevBlock, timestamp, toAccount, fromAccount, amount):
    block = _
    return _

# Creates and stores the genesis block
# Genesis block index is 0. It has no previous block, so the all previous fields are empty strings or -1 values
# The genesis black has a timestamp of 0, an amount of 0, and no to or from addresses
def createGenesisBlock():
    genesisBlock =  createBlock(_, 0, '', '', 0)
    return _

# Checks if a block is valid
# A block is valid if:
#   - The index is the previous index + 1
#   - The previous hash is the same as the previous block's hash
#   - The hash is the same as the calculated hash
#   - The hash meets the PoW check
def isValidBlock(newBlock, prevBlock):
    return _

# Adds a new block to the blockchain
# A block is added if it is valid
def addBlock(newBlock):
    if (_):
        return _
    return False