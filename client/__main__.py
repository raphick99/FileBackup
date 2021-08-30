from environment import Environment
from client import Client
from request_builder import Op


def main():
    env = Environment()
    c = Client(env)

    print(c.execute_request(Op.ListFiles))  # request to list files

    # request to back up all files in backup.info
    for backed_up_file_name in env.backed_up_files:
        with open(backed_up_file_name, 'r') as f:
            backed_up_file_content = f.read()
        print(c.execute_request(Op.BackupFile, filename=backed_up_file_name, payload=backed_up_file_content))

    print(c.execute_request(Op.ListFiles))  # list files again. should contain our new files

    backed_up_file = env.backed_up_files[0]
    recover_response = c.execute_request(Op.RecoverFile, filename=backed_up_file)  # recover first file
    print(recover_response)
    with open(f'{backed_up_file}.tmp', 'w') as f:
        f.write(str(recover_response.payload, 'utf-8'))

    print(c.execute_request(Op.DeleteBackedUpFile, filename=backed_up_file))  # delete the recovered file
    print(c.execute_request(Op.RecoverFile, filename=backed_up_file))  # attempt to recover it again



if __name__ == '__main__':
    main()
